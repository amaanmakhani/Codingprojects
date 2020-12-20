CREATE TABLE parts(
    pid         integer         NOT NULL,
    pname       varchar(40)     NOT NULL,
    color       varchar(20)     NOT NULL        CHECK (color ~ '^[a-zA-Z]+[-][0-9]+'),
    PRIMARY KEY(pid),
    check ((LENGTH(pname) % 2) = (LENGTH(color) % 2))
);

--Check if pid can be null
INSERT INTO parts(pname, color) VALUES ('test','abc-123');
--Check if pname can be null
INSERT INTO parts(pid, color) VALUES (2,'abc-123');
--Check if color can be null
INSERT INTO parts(pid, pname) VALUES (2, 'test');
--Check incorrect color format
INSERT INTO parts(pid, pname, color) VALUES (2, 'test', '123-ab');
--Check incorrect color format
INSERT INTO parts(pid, pname, color) VALUES (2, 'test', 'ab');
--Check incorrect color format
INSERT INTO parts(pid, pname, color) VALUES (2, 'test', '1');
--Check incorrect color format
INSERT INTO parts(pid, pname, color) VALUES (2, 'test', '');
--Check incorrect color format
INSERT INTO parts(pid, pname, color) VALUES (2, 'test', 'a2bc-123');
--Check if pname and color can be of different odd and even classes
INSERT INTO parts(pid, pname, color) VALUES (2, 'test', 'abc-123');

--Insert even pname and color length
INSERT INTO parts(pid, pname, color) VALUES (1, 'Nail', 'N-11');
--Insert odd pname and color length
INSERT INTO parts(pid, pname, color) VALUES (2, 'Hammer', 'H-11');
--Insert correct rows
INSERT INTO parts(pid, pname, color) VALUES (3, 'Screw', 'S-1');
INSERT INTO parts(pid, pname, color) VALUES (4, 'Drill', 'D-1');
INSERT INTO parts(pid, pname, color) VALUES (5, 'Knife', 'K-1');
INSERT INTO parts(pid, pname, color) VALUES (6, 'Bracket', 'B-1');
INSERT INTO parts(pid, pname, color) VALUES (7, 'Glue', 'G-11');
SELECT * FROM parts;

--No constraints as specified by instructor
CREATE TABLE partshistory(
    pid             integer         NOT NULL,
    pname           varchar(40)     NOT NULL,
    color           varchar(20)     NOT NULL,
    operation       CHAR(1)         NOT NULL,
    opwhen          TIMESTAMP       NOT NULL,
    opuser          CHAR(20)        NOT NULL
);

CREATE OR REPLACE FUNCTION track_parts_history() RETURNS TRIGGER AS $keep_part_history$
    BEGIN
        IF (TG_OP = 'DELETE') THEN
            INSERT INTO partshistory SELECT OLD.*, 'D', now(), user;
        ELSIF (TG_OP = 'UPDATE') THEN
            INSERT INTO partshistory SELECT OLD.*, 'U', now(), user;
        ELSIF (TG_OP = 'INSERT') THEN
            INSERT INTO partshistory SELECT NEW.*, 'I', now(), user;
        END IF;
        RETURN NULL;
    END;
$keep_part_history$ LANGUAGE plpgsql;

CREATE TRIGGER keep_part_history
AFTER INSERT OR UPDATE OR DELETE ON parts
    FOR EACH ROW EXECUTE PROCEDURE track_parts_history();

SELECT * FROM partshistory;

DELETE FROM parts WHERE pid = 1;
SELECT * FROM parts;
SELECT * FROM partshistory;

UPDATE parts SET pid = pid * 2 WHERE pid = 7;
SELECT * FROM parts;
SELECT * FROM partshistory;

INSERT INTO parts(pid, pname, color) VALUES (8, 'Nail', 'N-11');
SELECT * FROM parts;
SELECT * FROM partshistory;

\d parts
\d partshistory