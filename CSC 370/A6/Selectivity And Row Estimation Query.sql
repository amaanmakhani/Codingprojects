-- A single SQL query that computes the result of the estimation for using the data stored in the pg stats and pg class relations only 
-- The firstname is assumed to be the boundary of a histogram bucket
-- Fill in the below placeholders with your desired attributes:
--      <your relation name>
--      <your attribute name>
--      <your attribute value>

-- The generic query that you can fill in for use:
WITH userinput(relname, attname, attvalue) as
    (SELECT '<your relation name>', '<your attribute name>', '<your attribute value>'),
    -- Table below holds unnested mfv values and freqs
    mfvtable(mfv, mfvfreq) as
    (SELECT unnest((SELECT most_common_vals::text::text[] FROM pg_stats WHERE tablename = (select relname FROM userinput) AND attname = (select attname FROM userinput))), 
            unnest((SELECT most_common_freqs::text::real[] FROM pg_stats WHERE tablename = (select relname FROM userinput) AND attname = (select attname FROM userinput)))),
    -- Table below holds unnested histogram boundaries
    histogrambounds(bounds) as
    (SELECT unnest((SELECT histogram_bounds::text::text[] FROM pg_stats WHERE tablename = (select relname FROM userinput) AND attname = (select attname FROM userinput)))),
    -- Table below holds the portoion of the histogram after excluding null and MFV values
    histogramportion(portion) as
    (SELECT (1 - (SELECT null_frac FROM pg_stats WHERE tablename = (select relname FROM userinput) AND attname = (select attname FROM userinput)) - sum(mfvfreq))/100 FROM mfvtable),
    -- Table below holds the num of buckets needed
    mfvstoinclude(additionalfreq) as
    (SELECT sum(mfvfreq) FROM mfvtable WHERE mfv < (select attvalue FROM userinput)),
    -- Table below holds general calcuation values needed
    calculationvalues(numBuckets, numTuples) as 
    (SELECT COUNT(*), (SELECT reltuples FROM pg_class WHERE relname = (select relname FROM userinput)) FROM histogrambounds WHERE bounds < (select attvalue FROM userinput))
    -- Output data using values from the created tables
    SELECT numTuples as rows, ((numBuckets * portion) + additionalfreq) as selectivity, ((numBuckets * portion) + additionalfreq) * numTuples as expected_rows_unrounded, ROUND(((numBuckets * portion) + additionalfreq) * numTuples) expected_rows_rounded
    FROM calculationvalues NATURAL JOIN histogramportion NATURAL JOIN mfvstoinclude;

-- A filled in example
WITH userinput(relname, attname, attvalue) as
    (SELECT 'persons', 'firstname', 'Alejandro'),
    -- Table below holds unnested mfv values and freqs
    mfvtable(mfv, mfvfreq) as
    (SELECT unnest((SELECT most_common_vals::text::text[] FROM pg_stats WHERE tablename = (select relname FROM userinput) AND attname = (select attname FROM userinput))), 
            unnest((SELECT most_common_freqs::text::real[] FROM pg_stats WHERE tablename = (select relname FROM userinput) AND attname = (select attname FROM userinput)))),
    -- Table below holds unnested histogram boundaries
    histogrambounds(bounds) as
    (SELECT unnest((SELECT histogram_bounds::text::text[] FROM pg_stats WHERE tablename = (select relname FROM userinput) AND attname = (select attname FROM userinput)))),
    -- Table below holds the portoion of the histogram after excluding null and MFV values
    histogramportion(portion) as
    (SELECT (1 - (SELECT null_frac FROM pg_stats WHERE tablename = (select relname FROM userinput) AND attname = (select attname FROM userinput)) - sum(mfvfreq))/100 FROM mfvtable),
    -- Table below holds the num of buckets needed
    mfvstoinclude(additionalfreq) as
    (SELECT sum(mfvfreq) FROM mfvtable WHERE mfv < (select attvalue FROM userinput)),
    -- Table below holds general calcuation values needed
    calculationvalues(numBuckets, numTuples) as 
    (SELECT COUNT(*), (SELECT reltuples FROM pg_class WHERE relname = (select relname FROM userinput)) FROM histogrambounds WHERE bounds < (select attvalue FROM userinput))
    -- Output data using values from the created tables
    SELECT numTuples as rows, ((numBuckets * portion) + additionalfreq) as selectivity, ((numBuckets * portion) + additionalfreq) * numTuples as expected_rows_unrounded, ROUND(((numBuckets * portion) + additionalfreq) * numTuples) expected_rows_rounded
    FROM calculationvalues NATURAL JOIN histogramportion NATURAL JOIN mfvstoinclude;

-- The output from the above query
    rows     |    selectivity     | expected_rows_unrounded | expected_rows_rounded 
-------------+--------------------+-------------------------+-----------------------
 3.98471e+06 | 0.0200253273360431 |        79795.2021905137 |                 79795
(1 row)