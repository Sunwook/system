BEGIN TRANSACTION;
DROP TABLE IF EXISTS Reservations;
DROP TABLE IF EXISTS Customers;

CREATE TABLE IF NOT EXISTS Customers(CustomerId INTEGER PRIMARY KEY, Name TEXT);
INSERT INTO Customers(Name) VALUES('Paul Novak');
INSERT INTO Customers(Name) VALUES('Terry Neils');
INSERT INTO Customers(Name) VALUES('Jack Fonda');
INSERT INTO Customers(Name) VALUES('Tom Willis');

CREATE TABLE IF NOT EXISTS Reservations(Id INTEGER PRIMARY KEY, 
		    CustomerId INTEGER, Day TEXT);
INSERT INTO Reservations(CustomerId, Day) VALUES(1, '2009-22-11');
INSERT INTO Reservations(CustomerId, Day) VALUES(2, '2009-28-11');
INSERT INTO Reservations(CustomerId, Day) VALUES(2, '2009-29-11');
INSERT INTO Reservations(CustomerId, Day) VALUES(1, '2009-29-11');
INSERT INTO Reservations(CustomerId, Day) VALUES(3, '2009-02-12');
COMMIT;
