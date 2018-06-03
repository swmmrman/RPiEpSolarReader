class register {

}

/*
Obtained registers from mysql dump contained in the git of EpsolarServer.  The
git is located at https://github.com/danieloneill/EpsolarServer.git

INSERT INTO `registers` VALUES
(1,12544,'Charge voltage','V',0.010,'SOLE'),
(2,12545,'Charge current','A',0.010,'SOLE'),
(3,12546,'Charge watts','W',0.010,'SOLE'),
(4,12556,'Load voltage','V',0.010,'SOLE'),
(5,12557,'Load current','A',0.010,'SOLE'),
(6,12558,'Load watts','W',0.010,'SOLE'),
(7,12800,'Battery status',NULL,1.000,'BITMAP'),
(8,12801,'Charge controller status',NULL,1.000,'BITMAP'),
(9,12570,'Battery SOC','%',1.000,'SOLE'),
(10,12573,'Battery temperature','C',0.010,'SOLE'),
(11,13060,'Consumed energy today','Wh',100.000,'LOW'),
(12,13061,'Consumed energy today','Wh',100.000,'HIGH'),
(13,13068,'Generated energy today','Wh',100.000,'LOW'),
(14,13069,'Generated energy today','Wh',100.000,'HIGH'),
(15,13074,'Generated energy total','KWh',0.010,'LOW'),
(16,13075,'Generated energy total','KWh',0.010,'HIGH'),
(17,13076,'CO2 reduction','Kg',100.000,'LOW'),
(18,13077,'CO2 reduction','Kg',100.000,'HIGH');
