-- SQL for the Friends table

BEGIN TRANSACTION;
DROP TABLE IF EXISTS Friends;

CREATE TABLE Friends(Id INTEGER PRIMARY KEY, Name TEXT UNIQUE NOT NULL, 
		                     Sex TEXT CHECK(Sex IN ('M', 'F')));
INSERT INTO Friends VALUES(1, 'Jane', 'F');
INSERT INTO Friends VALUES(2, 'Thomas', 'M');
INSERT INTO Friends VALUES(3, 'Franklin', 'M');
INSERT INTO Friends VALUES(4, 'Elisabeth', 'F');
INSERT INTO Friends VALUES(5, 'Mary', 'F');
INSERT INTO Friends VALUES(6, 'Lucy', 'F');
INSERT INTO Friends VALUES(7, 'Jack', 'M');
COMMIT;
