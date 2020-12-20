--User defined function for report generation in conjunction with A4.py
CREATE FUNCTION am_udfA4(TEXT) RETURNS SETOF dirmovies AS $$
    SELECT productions.id, productions.year, ratings.rank, ratings.votes FROM productions
    LEFT JOIN ratings
    ON productions.id = ratings.id
    WHERE productions.id IN ( select id FROM directors WHERE pid = $1) AND productions.attr IS NULL
    ORDER BY year;
$$ LANGUAGE SQL;