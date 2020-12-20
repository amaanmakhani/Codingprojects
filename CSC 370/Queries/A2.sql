--Question 1

SELECT id, location FROM locations WHERE location = 'University of Victoria, Victoria, British Columbia';

-- Result
--         id         |                      location
----------------------+----------------------------------------------------
-- Emile (2003)       | University of Victoria, Victoria, British Columbia
-- Pink Ludoos (2004) | University of Victoria, Victoria, British Columbia
--(2 rows)


--Question 2

(SELECT id FROM Directors WHERE pid = 'Cameron, James (I)'
INTERSECT
SELECT id FROM Productions WHERE 1991 <= year AND year <= 2009)
UNION
(SELECT id FROM roles WHERE pid = 'Winslet, Kate'
INTERSECT
SELECT id FROM Productions WHERE year = 1997);

-- Result
--                                      id
--------------------------------------------------------------------------------
-- "HBO First Look" (1992) {Heart of the Ocean: The Making of 'Titanic' (#4.9)}
-- Titanic (1997)
-- Terminator 2: Judgment Day (1991)
-- Ghosts of the Abyss (2003)
-- T2 3-D Pre-Show (1996)
-- To Be on Camera: A History with Hamlet (1997) (V)
-- Avatar (2009)
-- "HBO First Look" (1992) {Titanic}
-- Aliens of the Deep (2005)
-- T2 3-D: Battle Across Time (1996)
-- "Gomorron" (1992) {Om filmen 'Titanic'}
-- "Dark Angel" (2000) {Freak Nation (#2.21)}
-- True Lies (1994)
-- Earthship.TV (2001) (TV)
-- "Magacine" (1997)
-- Expedition: Bismarck (2002) (TV)
--(16 rows)


-- Question 3

SELECT id FROM Directors WHERE pid = 'Eastwood, Clint'
INTERSECT
SELECT id FROM Roles WHERE pid = 'Eastwood, Clint'
INTERSECT
SELECT id FROM Productions WHERE attr IS NULL;

-- Result
--                  id
----------------------------------------
-- Firefox (1982)
-- Heartbreak Ridge (1986)
-- Bronco Billy (1980)
-- Absolute Power (1997)
-- High Plains Drifter (1973)
-- Unforgiven (1992)
-- Space Cowboys (2000)
-- The Gauntlet (1977)
-- Breezy (1973)
-- Million Dollar Baby (2004)
-- Pale Rider (1985)
-- Sudden Impact (1983)
-- Blood Work (2002)
-- Play Misty for Me (1971)
-- The Rookie (1990)
-- The Eiger Sanction (1975)
-- Gran Torino (2008)
-- White Hunter Black Heart (1990)
-- A Perfect World (1993)
-- The Bridges of Madison County (1995)
-- The Outlaw Josey Wales (1976)
-- Honkytonk Man (1982)
-- True Crime (1999)
--(23 rows)


--Question 4

SELECT id FROM Directors WHERE pid = 'Eastwood, Clint'
EXCEPT
SELECT id FROM Roles WHERE pid = 'Eastwood, Clint';

-- Result
--                            id
------------------------------------------------------------
-- Changeling (2008)
-- Flags of Our Fathers (2006)
-- Sully (2016)
-- The Ballad of Richard Jewell (????)
-- Bird (1988)
-- Jersey Boys (2014)
-- Hereafter (2010)
-- Mystic River (2003)
-- Invictus (2009)
-- "Amazing Stories" (1985) {Vanessa in the Garden (#1.12)}
-- Letters from Iwo Jima (2006)
-- The Beguiled: The Storyteller (1971)
-- Midnight in the Garden of Good and Evil (1997)
-- American Sniper (2014)
-- J. Edgar (2011)
--(15 rows)


--Question 5

SELECT id, character, billing FROM roles
WHERE pid = 'Eastwood, Clint' AND id IN(
SELECT id FROM Directors WHERE pid = 'Leone, Sergio (I)'
INTERSECT
SELECT id FROM Roles WHERE pid = 'Eastwood, Clint'
);

-- Result
--                   id                   | character | billing
------------------------------------------+-----------+---------
-- Per un pugno di dollari (1964)         | Joe       |       1
-- Per qualche dollaro in più (1965)      | Monco     |       1
-- Il buono, il brutto, il cattivo (1966) | Blondie   |       2
--(3 rows)


--Question 6

SELECT id, pid, character FROM roles
WHERE (id = 'The Matrix Reloaded (2003)' OR id = 'The Matrix (1999)') AND pid IN(   
SELECT pid FROM Roles WHERE id = 'The Matrix Reloaded (2003)'
INTERSECT
SELECT pid FROM Roles WHERE id = 'The Matrix (1999)'
);

-- Result
--             id             |         pid         |  character
------------------------------+---------------------+-------------
-- The Matrix (1999)          | Fishburne, Laurence | Morpheus
-- The Matrix (1999)          | Foster, Gloria (I)  | Oracle
-- The Matrix (1999)          | Moss, Carrie-Anne   | Trinity
-- The Matrix (1999)          | Reeves, Keanu       | Neo
-- The Matrix (1999)          | Weaving, Hugo       | Agent Smith
-- The Matrix Reloaded (2003) | Fishburne, Laurence | Morpheus
-- The Matrix Reloaded (2003) | Foster, Gloria (I)  | The Oracle
-- The Matrix Reloaded (2003) | Moss, Carrie-Anne   | Trinity
-- The Matrix Reloaded (2003) | Reeves, Keanu       | Neo
-- The Matrix Reloaded (2003) | Weaving, Hugo       | Agent Smith
--(10 rows)


--Question 7

SELECT id, pid FROM directors
WHERE id IN(
SELECT id FROM Roles WHERE pid = 'Radcliffe, Daniel'
INTERSECT
SELECT id FROM Roles WHERE pid = 'Watson, Emma (II)'
INTERSECT
SELECT id FROM Productions WHERE attr is NULL
);

-- Result
--                         id                          |        pid
-------------------------------------------------------+-------------------
-- Harry Potter and the Sorcerer's Stone (2001)        | Columbus, Chris
-- Harry Potter and the Deathly Hallows: Part 1 (2010) | Yates, David (II)
-- Harry Potter and the Prisoner of Azkaban (2004)     | Cuarón, Alfonso
-- Harry Potter and the Goblet of Fire (2005)          | Newell, Mike (I)
-- Harry Potter and the Escape from Gringotts (2014)   | Coup, Thierry
-- Harry Potter and the Forbidden Journey (2010)       | Coup, Thierry
-- Harry Potter and the Half-Blood Prince (2009)       | Yates, David (II)
-- Harry Potter and the Order of the Phoenix (2007)    | Yates, David (II)
-- Harry Potter and the Deathly Hallows: Part 2 (2011) | Yates, David (II)
-- Harry Potter and the Chamber of Secrets (2002)      | Columbus, Chris
--(10 rows)