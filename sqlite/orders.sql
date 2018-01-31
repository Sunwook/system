-- SQL for the Orders table

BEGIN TRANSACTION;
DROP TABLE IF EXISTS Orders;

CREATE TABLE Orders(Id INTEGER PRIMARY KEY, 
		                 OrderPrice INTEGER CHECK(OrderPrice>0), Customer TEXT);
INSERT INTO Orders(OrderPrice, Customer) VALUES(1200, "Williamson");
INSERT INTO Orders(OrderPrice, Customer) VALUES(200, "Robertson");
INSERT INTO Orders(OrderPrice, Customer) VALUES(40, "Robertson");
INSERT INTO Orders(OrderPrice, Customer) VALUES(1640, "Smith");
INSERT INTO Orders(OrderPrice, Customer) VALUES(100, "Robertson");
INSERT INTO Orders(OrderPrice, Customer) VALUES(50, "Williamson");
INSERT INTO Orders(OrderPrice, Customer) VALUES(150, "Smith");
INSERT INTO Orders(OrderPrice, Customer) VALUES(250, "Smith");
INSERT INTO Orders(OrderPrice, Customer) VALUES(840, "Brown");
INSERT INTO Orders(OrderPrice, Customer) VALUES(440, "Black");
INSERT INTO Orders(OrderPrice, Customer) VALUES(20, "Brown");
COMMIT;
