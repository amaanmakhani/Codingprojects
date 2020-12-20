--Question 1

SELECT title, info.pid, year, info.rank FROM productions
INNER JOIN
(SELECT pop.id ,femdir.pid, pop.rank FROM (SELECT directors.id, directors.pid FROM directors
INNER JOIN persons
ON persons.pid = directors.pid
WHERE persons.gender = 'F' AND persons.lastname = 'Bigelow'
) AS femdir
INNER JOIN (SELECT id, rank FROM ratings
WHERE votes > 50000) AS pop
ON pop.id = femdir.id) AS info
ON productions.id = info.id;

-- Result
--      title       |       pid        | year | rank
--------------------+------------------+------+------
-- The Hurt Locker  | Bigelow, Kathryn | 2008 |  7.6
-- Point Break      | Bigelow, Kathryn | 1991 |  7.2
-- Strange Days     | Bigelow, Kathryn | 1995 |  7.2
-- Zero Dark Thirty | Bigelow, Kathryn | 2012 |  7.4
--(4 rows)


--Question 2

SELECT a.id AS title, d.rank AS rank, c.year AS year, b.character AS emmachar, b.billing AS emmabilling, a.character as danielchar, a.billing AS danielbilling FROM roles AS a
INNER JOIN (SELECT id, character, billing FROM roles WHERE pid = 'Watson, Emma (II)') AS b ON a.id = b.id
INNER JOIN (SELECT id, year FROM productions WHERE attr IS NULL) AS c ON a.id = c.id
INNER JOIN (SELECT id, rank FROM ratings) AS d ON a.id = d.id
WHERE a.pid = 'Radcliffe, Daniel' AND NOT a.character ~ 'Himself' AND NOT b.character ~ 'Herself';

-- Result
--                        title                        | rank | year |     emmachar     | emmabilling |  danielchar  | danielbilling
-------------------------------------------------------+------+------+------------------+-------------+--------------+---------------
-- Harry Potter and the Chamber of Secrets (2002)      |  7.4 | 2002 | Hermione Granger |           3 | Harry Potter |             1
-- Harry Potter and the Deathly Hallows: Part 1 (2010) |  7.7 | 2010 | Hermione Granger |           2 | Harry Potter |             5
-- Harry Potter and the Deathly Hallows: Part 2 (2011) |  8.1 | 2011 | Hermione Granger |           6 | Harry Potter |             4
-- Harry Potter and the Escape from Gringotts (2014)   |  8.9 | 2014 | Hermione Granger |             | Harry Potter |
-- Harry Potter and the Forbidden Journey (2010)       |    8 | 2010 | Hermione Granger |             | Harry Potter |
-- Harry Potter and the Goblet of Fire (2005)          |  7.7 | 2005 | Hermione Granger |           5 | Harry Potter |             4
-- Harry Potter and the Half-Blood Prince (2009)       |  7.5 | 2009 | Hermione Granger |          10 | Harry Potter |             1
-- Harry Potter and the Order of the Phoenix (2007)    |  7.5 | 2007 | Hermione Granger |          25 | Harry Potter |             1
-- Harry Potter and the Prisoner of Azkaban (2004)     |  7.8 | 2004 | Hermione Granger |          16 | Harry Potter |             1
-- Harry Potter and the Sorcerer's Stone (2001)        |  7.5 | 2001 | Hermione Granger |          25 | Harry Potter |             5
--(10 rows)


--Question 3

SELECT id, year FROM productions
WHERE attr = 'TV-series' AND year >= 2000 AND id IN
(SELECT DISTINCT episodeof FROM episodes
WHERE id IN
(SELECT id FROM productions WHERE attr = 'TV-ep'
INTERSECT
SELECT id FROM locations WHERE location = 'Victoria, British Columbia'));

-- Result
--                 id                  | year
---------------------------------------+------
-- "Cedar Cove" (2013)                 | 2013
-- "Creepy Canada" (2002)              | 2002
-- "Eaux troubles du crime" (2007)     | 2007
-- "Glutton for Punishment" (2007)     | 2007
-- "Improbabilia" (2013)               | 2013
-- "Senior Living on Location" (2012)  | 2012
-- "Spooksville" (2013)                | 2013
-- "The Dead Zone" (2002)              | 2002
-- "World's Most Extreme Homes" (2006) | 2006
--(9 rows)


--Question 4

SELECT b.pid, b.id, d.rank FROM languages
INNER JOIN (SELECT id, pid FROM directors WHERE pid IN
(SELECT pid FROM directors WHERE id IN (SELECT id FROM episodes WHERE episodeof = '"Hora Marcada" (1986)')))
AS b ON languages.id = b.id
INNER JOIN (SELECT id FROM productions WHERE attr IS NULL) AS c ON c.id = b.id
LEFT JOIN (SELECT id, rank FROM ratings) AS d ON c.id = d.id
WHERE language = 'English';

-- Result
--           pid            |                       id                        | rank
----------------------------+-------------------------------------------------+------
-- del Toro, Guillermo      | The Haunted Mansion (????)                      |
-- del Toro, Guillermo      | The Shape of Water (2017)                       |
-- del Toro, Guillermo      | The Witches (????)                              |
-- Cuarón, Alfonso          | Vengeance Is Mine (1983)                        |  7.4
-- Silva, Batan             | After Darkness (2016)                           |
-- Cuarón, Alfonso          | A Little Princess (1995)                        |  7.7
-- del Toro, Guillermo      | At the Mountains of Madness (????)              |
-- de la Riva, Juan Antonio | Aztech (2017)                                   |
-- del Toro, Guillermo      | Blade II (2002)                                 |  6.7
-- Gurrola, Alfredo (I)     | Cabalgando con la muerte (1989)                 |  6.2
-- Cuarón, Alfonso          | Children of Men (2006)                          |  7.9
-- del Toro, Guillermo      | Crimson Peak (2015)                             |  6.6
-- del Toro, Guillermo      | Cronos (1993)                                   |  6.7
-- Cuarón, Alfonso          | Gravity (2013)                                  |  7.8
-- Cuarón, Alfonso          | Great Expectations (1998)                       |  6.8
-- del Toro, Guillermo      | Hellboy (2004)                                  |  6.8
-- del Toro, Guillermo      | Hellboy II: The Golden Army (2008)              |    7
-- Cuarón, Alfonso          | Harry Potter and the Prisoner of Azkaban (2004) |  7.8
-- del Toro, Guillermo      | Hellboy 3 (????)                                |
-- Cuarón, Alfonso          | Paris, je t'aime (2006)                         |  7.3
-- del Toro, Guillermo      | Mimic (1997)                                    |  5.9
-- del Toro, Guillermo      | Pacific Rim (2013)                              |    7
-- del Toro, Guillermo      | Pinocchio (????/II)                             |
-- del Toro, Guillermo      | Saturn and the End of Days (????)               |
-- Cuarón, Alfonso          | Sólo con tu pareja (1991)                       |  7.1
--(25 rows)


--Question 5

SELECT id AS lucas, idlinkedto AS Kurosawa, relationship FROM links
WHERE id
IN (SELECT id FROM directors WHERE pid = 'Lucas, George (I)')
AND idlinkedto
IN (SELECT id FROM directors WHERE pid = 'Kurosawa, Akira');

-- Result
--                        lucas                        |              kurosawa               | relationship
-------------------------------------------------------+-------------------------------------+--------------
-- Star Wars: Episode I - The Phantom Menace (1999)    | Kagemusha (1980)                    | references
-- Star Wars: Episode I - The Phantom Menace (1999)    | Kakushi-toride no san-akunin (1958) | references
-- Star Wars: Episode I - The Phantom Menace (1999)    | Kumonosu-jô (1957)                  | references
-- Star Wars: Episode I - The Phantom Menace (1999)    | Shichinin no samurai (1954)         | references
-- Star Wars: Episode II - Attack of the Clones (2002) | Kagemusha (1980)                    | references
-- Star Wars: Episode II - Attack of the Clones (2002) | Shichinin no samurai (1954)         | references
-- Star Wars: Episode III - Revenge of the Sith (2005) | Shichinin no samurai (1954)         | references
-- Star Wars (1977)                                    | Dersu Uzala (1975)                  | references
-- Star Wars (1977)                                    | Kakushi-toride no san-akunin (1958) | references
-- Star Wars (1977)                                    | Shichinin no samurai (1954)         | references
-- Star Wars (1977)                                    | Yôjinbô (1961)                      | references
--(11 rows)


--Question 6

SELECT a.title, a.year, rank, votes FROM ratings
INNER JOIN (SELECT id, title, year FROM productions WHERE attr IS NULL) AS a ON a.id = ratings.id
WHERE rank = (SELECT MAX(rank) FROM ratings WHERE id IN (SELECT id FROM productions WHERE attr IS NULL) AND votes > 50000) AND votes > 50000;

-- Result
--          title           | year | rank |  votes
----------------------------+------+------+---------
-- The Shawshank Redemption | 1994 |  9.3 | 1698604
--(1 row)


--Question 7

SELECT season, COUNT(epnumber), MAX(a.rank) AS maxrank, MIN(a.rank) AS minrank, AVG(a.rank) AS avgrank, SUM(a.votes) AS totalvote FROM episodes
INNER JOIN (SELECT id, rank, votes FROM ratings) AS a ON a.id = episodes.id
WHERE episodeof = '"The Simpsons" (1989)'
GROUP BY season
HAVING COUNT(epnumber) > 0
ORDER BY season;

-- Result
-- season | count | maxrank | minrank |     avgrank      | totalvote
----------+-------+---------+---------+------------------+-----------
--      1 |    13 |     8.3 |     7.4 | 7.80769230769231 |     23540
--      2 |    22 |     8.8 |     7.5 | 8.04090909090909 |     30915
--      3 |    24 |     8.8 |     7.7 | 8.15833333333333 |     31197
--      4 |    22 |       9 |     7.2 | 8.27272727272727 |     29357
--      5 |    22 |       9 |     7.5 | 8.34090909090909 |     27563
--      6 |    25 |     9.1 |       6 |             8.32 |     30536
--      7 |    25 |       9 |     7.5 |            8.332 |     29354
--      8 |    25 |     9.2 |     7.3 |             8.22 |     30564
--      9 |    25 |     9.1 |     5.1 |            7.844 |     25152
--     10 |    23 |     8.2 |     6.9 | 7.57391304347826 |     20102
--     11 |    22 |     7.9 |     6.6 |              7.3 |     17288
--     12 |    21 |     8.6 |     6.5 | 7.37142857142857 |     16015
--     13 |    22 |       8 |     5.6 | 7.13636363636364 |     14649
--     14 |    22 |     7.7 |     6.6 | 7.07272727272727 |     13669
--     15 |    22 |     7.4 |     6.2 | 7.03636363636364 |     13049
--     16 |    21 |     7.4 |     6.4 | 7.04285714285714 |     11966
--     17 |    22 |     7.4 |     6.3 | 6.86363636363636 |     13090
--     18 |    22 |     8.1 |     6.3 | 7.00454545454545 |     14249
--     19 |    20 |     8.2 |     6.1 |            6.925 |     13431
--     20 |    21 |     7.3 |     5.9 |  6.8952380952381 |     13202
--     21 |    23 |     7.3 |     5.7 | 6.82173913043478 |     12348
--     22 |    22 |     7.2 |     6.1 | 6.79545454545455 |     11106
--     23 |    22 |     8.2 |     4.5 | 6.80454545454545 |     12545
--     24 |    22 |     7.3 |     6.2 | 6.76818181818182 |      9505
--     25 |    22 |     7.7 |     5.8 | 6.82272727272727 |     10515
--     26 |    22 |     7.9 |     5.8 | 6.70454545454545 |      9783
--     27 |    22 |     8.4 |       6 |              6.8 |      6778
--(27 rows)


--Question 8

SELECT a.pid, COUNT(a.pid) AS count,AVG(temp.rank) FROM (SELECT id, pid FROM roles WHERE (pid = 'McGowan, Mickie' OR pid = 'Flowers, Bess' OR pid = 'Lynn, Sherry (I)')) AS a
INNER JOIN
(SELECT id, rank FROM ratings WHERE id IN
(SELECT id FROM roles WHERE (pid = 'McGowan, Mickie' OR pid = 'Flowers, Bess' OR pid = 'Lynn, Sherry (I)')
AND id IN (SELECT id FROM ratings WHERE votes >= 50000 AND rank >= 8))) AS temp
ON temp.id = a.id
GROUP BY a.pid
ORDER BY AVG(temp.rank);

-- Result
--       pid        | count |       avg
--------------------+-------+------------------
-- McGowan, Mickie  |    10 |             8.23
-- Lynn, Sherry (I) |    12 | 8.25833333333333
-- Flowers, Bess    |    12 | 8.25833333333333
--(3 rows)


--Question 9

SELECT pid, id, billing, character FROM roles
WHERE pid IN (SELECT pid FROM roles
WHERE id IN (SELECT id FROM ratings
WHERE votes > 250000)
GROUP BY pid
ORDER by COUNT(pid) DESC
LIMIT 1) AND id IN (SELECT id FROM ratings
WHERE votes > 250000);

-- Result
--      pid      |                     id                     | billing |             character
-----------------+--------------------------------------------+---------+------------------------------------
-- Lee, Stan (I) | Ant-Man (2015)                             |      62 | Bartender
-- Lee, Stan (I) | Avengers: Age of Ultron (2015)             |      22 | Stan Lee
-- Lee, Stan (I) | Big Hero 6 (2014)                          |      43 | Fred's Dad
-- Lee, Stan (I) | Captain America: Civil War (2016)          |      85 | FedEx Driver
-- Lee, Stan (I) | Captain America: The First Avenger (2011)  |         | General
-- Lee, Stan (I) | Captain America: The Winter Soldier (2014) |      13 | Smithsonian Guard
-- Lee, Stan (I) | Deadpool (2016)                            |      30 | Strip Club DJ
-- Lee, Stan (I) | Fantastic Four (2005/I)                    |      46 | Willie Lumpkin
-- Lee, Stan (I) | Guardians of the Galaxy (2014)             |      28 | Xandarian Ladies' Man
-- Lee, Stan (I) | Iron Man (2008)                            |      51 | Stan Lee
-- Lee, Stan (I) | Iron Man 2 (2010)                          |      28 | Stan Lee
-- Lee, Stan (I) | Iron Man Three (2013)                      |      52 | Pageant Judge
-- Lee, Stan (I) | Spider-Man (2002)                          |         | Man in Fair
-- Lee, Stan (I) | Spider-Man 2 (2004)                        |      24 | Man Dodging Debris
-- Lee, Stan (I) | Spider-Man 3 (2007)                        |      23 | Man in Times Square
-- Lee, Stan (I) | The Amazing Spider-Man (2012)              |      24 | School Librarian
-- Lee, Stan (I) | The Amazing Spider-Man 2 (2014)            |      58 | Graduation Guest
-- Lee, Stan (I) | The Avengers (2012)                        |      51 | Stan Lee
-- Lee, Stan (I) | The Incredible Hulk (2008)                 |         | Milwaukee Man Drinking From Bottle
-- Lee, Stan (I) | Thor (2011)                                |      35 | Stan the Man
-- Lee, Stan (I) | Thor: The Dark World (2013)                |      35 | Stan Lee
-- Lee, Stan (I) | X-Men (2000)                               |      36 | Hot Dog Vendor
-- Lee, Stan (I) | X-Men: The Last Stand (2006)               |      26 | Waterhose Man
--(23 rows)


--Question 10

SELECT a.pid, ((a.c * 1.0)/(b.c * 1.0)) * 100.0 AS prop, b.c AS total, a.c AS goodones, a.r AS avggoodones, b.c - a.c AS rest, d.r AS avgrest, a.r - d.r AS diff FROM
(SELECT pid, COUNT(pid) AS c, AVG(c.rank) AS r FROM directors
INNER JOIN (SELECT id, rank FROM ratings WHERE votes > 50000 AND rank >= 8.5 AND id IN (SELECT id FROM Productions WHERE attr IS NULL)) AS c
ON directors.id = c.id
GROUP BY pid) AS a
INNER JOIN (SELECT pid, COUNT(pid) AS c FROM directors WHERE id IN (SELECT id FROM Productions WHERE attr IS NULL) AND id IN (SELECT id FROM ratings WHERE votes > 50000)
GROUP BY pid HAVING COUNT(pid) >= 5) AS b
ON a.pid = b.pid
INNER JOIN (SELECT pid, AVG(c.rank) AS r FROM directors
INNER JOIN (SELECT id, rank FROM ratings WHERE rank < 8.5 AND votes > 50000 AND id IN (SELECT id FROM Productions WHERE attr IS NULL)) AS c
ON directors.id = c.id
GROUP BY pid) AS d
ON a.pid = d.pid
ORDER BY prop DESC;

-- Result
--          pid           |           prop           | total | goodones |   avggoodones    | rest |     avgrest      |       diff
--------------------------+--------------------------+-------+----------+------------------+------+------------------+-------------------
-- Nolan, Christopher (I) | 66.666666666666666667000 |     9 |        6 |             8.65 |    3 |              7.7 |              0.95
-- Chaplin, Charles       | 60.000000000000000000000 |     5 |        3 | 8.56666666666667 |    2 |             8.25 | 0.316666666666668
-- Coppola, Francis Ford  | 42.857142857142857143000 |     7 |        3 |              8.9 |    4 |             7.55 |              1.35
-- Leone, Sergio (I)      | 40.000000000000000000000 |     5 |        2 |             8.75 |    3 | 8.23333333333333 | 0.516666666666666
-- Jackson, Peter (I)     | 27.272727272727272727000 |    11 |        3 |              8.8 |    8 |           7.4125 |            1.3875
-- Lumet, Sidney          | 20.000000000000000000000 |     5 |        1 |              8.9 |    4 |              7.8 |               1.1
-- Wilder, Billy          | 20.000000000000000000000 |     5 |        1 |              8.5 |    4 |             8.35 | 0.149999999999999
-- Fincher, David         | 20.000000000000000000000 |    10 |        2 |              8.7 |    8 |           7.5125 |            1.1875
-- Hitchcock, Alfred (I)  | 20.000000000000000000000 |    10 |        2 |              8.5 |    8 |            8.125 |             0.375
-- Lucas, George (I)      | 20.000000000000000000000 |     5 |        1 |              8.7 |    4 |            7.075 |             1.625
-- Kubrick, Stanley       | 18.181818181818181818000 |    11 |        2 |              8.5 |    9 | 8.04444444444444 | 0.455555555555556
-- Wachowski, Lilly       | 16.666666666666666667000 |     6 |        1 |              8.7 |    5 |             6.58 |              2.12
-- Tarantino, Quentin     | 16.666666666666666667000 |    12 |        2 |              8.7 |   10 |             7.77 | 0.929999999999999
-- Besson, Luc            | 16.666666666666666667000 |     6 |        1 |              8.6 |    5 |             6.84 |              1.76
-- Polanski, Roman (I)    | 16.666666666666666667000 |     6 |        1 |              8.5 |    5 |             7.46 |              1.04
-- Wachowski, Lana        | 16.666666666666666667000 |     6 |        1 |              8.7 |    5 |             6.58 |              2.12
-- Cameron, James (I)     | 14.285714285714285714000 |     7 |        1 |              8.5 |    6 | 7.81666666666667 | 0.683333333333334
-- Zemeckis, Robert       | 13.333333333333333333000 |    15 |        2 |             8.65 |   13 |              7.1 |              1.55
-- Scorsese, Martin (I)   | 13.333333333333333333000 |    15 |        2 |              8.6 |   13 | 7.68461538461538 | 0.915384615384615
-- Singer, Bryan          | 12.500000000000000000000 |     8 |        1 |              8.6 |    7 |              7.1 |               1.5
-- Miyazaki, Hayao        | 12.500000000000000000000 |     8 |        1 |              8.6 |    7 | 8.08571428571429 | 0.514285714285714
-- Spielberg, Steven      | 12.000000000000000000000 |    25 |        3 | 8.66666666666667 |   22 | 7.43636363636364 |  1.23030303030303
-- Scott, Ridley          | 11.764705882352941176000 |    17 |        2 |              8.5 |   15 | 7.02666666666667 |  1.47333333333333
--(23 rows)