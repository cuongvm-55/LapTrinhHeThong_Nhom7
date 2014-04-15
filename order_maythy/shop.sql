-- MySQL dump 10.13  Distrib 5.5.35, for debian-linux-gnu (i686)
--
-- Host: localhost    Database: shop
-- ------------------------------------------------------
-- Server version	5.5.35-0ubuntu0.12.04.2

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `customer`
--

DROP TABLE IF EXISTS `customer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `customer` (
  `CustomerId` int(10) unsigned zerofill NOT NULL AUTO_INCREMENT,
  `Name` varchar(90) DEFAULT NULL,
  `Phone` varchar(20) DEFAULT NULL,
  `Address` varchar(120) DEFAULT NULL,
  `Email` varchar(90) DEFAULT NULL,
  `Gender` tinyint(1) DEFAULT NULL,
  `CreatedAt` timestamp NULL DEFAULT NULL,
  `UpdatedAt` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`CustomerId`)
) ENGINE=InnoDB AUTO_INCREMENT=65 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `customer`
--

LOCK TABLES `customer` WRITE;
/*!40000 ALTER TABLE `customer` DISABLE KEYS */;
INSERT INTO `customer` VALUES (0000000001,'4424242','01672642420','Xuân Đỉnh, Từ Liêm, Hà Nội','maythywinter@gmai.com',1,'2014-04-03 17:33:51','2014-04-14 17:24:22'),(0000000002,'Dat','01662329830','Mễ Trì Hạ, Từ Liêm, Hà Nội','datnq.55@gmail.com',0,'2014-04-03 17:34:17','2014-04-15 01:23:17'),(0000000003,'Thy','01663724852','Số nhà 1A, ngách 1/41, ngõ 1 Phạm Văn Đồng','cuongvm.55@gmail.com',1,'2014-04-03 17:34:37','2014-04-15 01:31:42'),(0000000004,'Nguyễn Văn Kháng','01645072798','Phòng 104 ký túc xá đại học Ngoại Ngữ','khangnv111@gmail.com',3,NULL,NULL),(0000000005,'May Thy','01672642421','okkkk','robert@vinova.vn',1,'2014-04-14 03:06:22','2014-04-14 03:06:22'),(0000000006,'Trần Quốc Huy','01692233083','Xuân Đỉnh','robert@vinova.vn',1,'2014-04-14 03:19:27','2014-04-14 03:19:27'),(0000000007,'Thy','01692233083','Xuân Đỉnh','robert@vinova.vn',1,'2014-04-14 03:20:31','2014-04-14 03:20:31'),(0000000008,'Thy','01692233083','Xuân Đỉnh','robert@vinova.vn',1,'2014-04-14 03:21:03','2014-04-14 03:21:03'),(0000000009,'Thy','01692233083','Xuân Đỉnh','robert@vinova.vn',1,'2014-04-14 03:21:07','2014-04-14 03:21:07'),(0000000010,'Clark','01667676666','sdfsf','fsf',0,'2014-04-14 03:32:26','2014-04-14 03:32:26'),(0000000011,'Clark','01667676666','sdfsf','fsf',0,'2014-04-14 03:32:57','2014-04-14 03:32:57'),(0000000012,'a','01667676666','sdfsf','fsf',0,'2014-04-14 03:33:17','2014-04-14 03:33:17'),(0000000013,'a','01667676666','sdfsf','fsf',1,'2014-04-14 03:33:22','2014-04-14 03:33:22'),(0000000014,'a','01667676666','sdfsf','fsf',1,'2014-04-14 03:33:24','2014-04-14 03:33:24'),(0000000015,'a','01667676666','sdfsf','fsf',1,'2014-04-14 03:33:27','2014-04-14 03:33:27'),(0000000016,'a','01667676666','sdfsf','fsf',0,'2014-04-14 03:33:29','2014-04-14 03:33:29'),(0000000017,'a','01667676666','sdfsf','fsf',1,'2014-04-14 03:33:32','2014-04-14 03:33:32'),(0000000018,'thyyyyyyyyy','09090909090','haahaha','ok@ok.com',0,'2014-04-14 04:08:47','2014-04-14 04:08:47'),(0000000019,'thyyyyyyyyy','09090909090','haahaha','ok@ok.com',0,'2014-04-14 04:09:01','2014-04-14 04:09:01'),(0000000020,'Mẫn Mẫn','01271271127','Ký túc xá Ngoại Ngữ','manman@gmail.com',0,'2014-04-14 04:21:19','2014-04-14 04:21:19'),(0000000021,'robert from china','01665665656','guangxi','maythy@w.cn',0,'2014-04-14 04:27:19','2014-04-14 04:27:19'),(0000000022,'thy','016623298309','mbjgkgu','ugigg',1,'2014-04-14 07:46:31','2014-04-14 07:47:09'),(0000000023,'fdsfsf','sfsf','','kjhijhg',0,'2014-04-14 07:50:42','2014-04-14 07:50:42'),(0000000024,'afaf','fsfs','','',0,'2014-04-14 09:17:02','2014-04-14 09:17:02'),(0000000025,'gsg','0142342423434','','',0,'2014-04-14 09:18:53','2014-04-14 09:18:53'),(0000000026,'faff','fsfsd','','',0,'2014-04-14 09:20:48','2014-04-14 09:20:48'),(0000000027,'fsf','fsfsdf','','',0,'2014-04-14 09:22:48','2014-04-14 09:22:48'),(0000000028,'fsfsf','fsfsf','','',0,'2014-04-14 10:04:44','2014-04-14 10:04:44'),(0000000029,'fsfsf','eft2rrrwerwrwe','','',0,'2014-04-14 10:04:52','2014-04-14 10:04:52'),(0000000030,'fsfasdadadadaddada','013421424242423','adsadad','',1,'2014-04-14 10:19:21','2014-04-14 10:21:08'),(0000000031,'fsfasdadadadaddada','013421424242423aa','adsadad','',1,'2014-04-14 10:21:21','2014-04-14 10:21:21'),(0000000032,'fsfasdadadadaddada','013421424242423aa','adsadad','',1,'2014-04-14 10:22:16','2014-04-14 10:22:16'),(0000000033,'fsfasdadadadaddadaa','01342142424','adsadad','',1,'2014-04-14 10:22:44','2014-04-14 10:22:44'),(0000000034,'man man man man','3492874298374','sdfsfsdf3rr','fsfs',0,'2014-04-14 10:34:19','2014-04-14 10:34:19'),(0000000035,'12312312','123123123123123','','',0,'2014-04-14 10:41:41','2014-04-14 10:41:41'),(0000000036,'adsaff','98989898989898','','',0,'2014-04-14 10:42:48','2014-04-14 10:42:48'),(0000000037,'afaf','1231231231231213333','','',0,'2014-04-14 10:49:41','2014-04-14 10:49:41'),(0000000038,'fsaf','09090999999','fsf','',0,'2014-04-14 11:01:37','2014-04-14 11:01:37'),(0000000039,'fsaf','090909999992','fsf','',0,'2014-04-14 11:01:52','2014-04-14 11:01:52'),(0000000040,'fafsdf','1241342353423423','','',0,'2014-04-14 11:07:31','2014-04-14 11:07:31'),(0000000041,'fsdf','928347239842','','',0,'2014-04-14 11:08:20','2014-04-14 11:08:20'),(0000000042,'fafd2','95390539583','','',0,'2014-04-14 11:09:36','2014-04-14 11:09:36'),(0000000043,'fsdf','423498247234','','',0,'2014-04-14 11:19:34','2014-04-14 11:19:34'),(0000000044,'afaf','fsdfsfsf23r234242','','',0,'2014-04-14 11:28:02','2014-04-14 11:28:02'),(0000000045,'fsfs','43228472364826472','dadadads','',0,'2014-04-14 11:28:32','2014-04-14 11:28:32'),(0000000046,'dad','dafdsfssfsf1321313','','',0,'2014-04-14 11:34:13','2014-04-14 11:34:13'),(0000000047,'fsdfs','1938193817831739817','','',0,'2014-04-14 11:35:38','2014-04-14 11:35:38'),(0000000048,'fsdfs','016726424201','','',0,'2014-04-14 11:36:13','2014-04-14 11:36:13'),(0000000049,'afaf','11111111111111111111','','',0,'2014-04-14 11:39:02','2014-04-14 11:39:02'),(0000000050,'sfsf','31983138129381923','','',0,'2014-04-14 11:41:42','2014-04-14 11:41:42'),(0000000051,'sfsf','31983138129381923','','',0,'2014-04-14 11:42:02','2014-04-14 11:42:02'),(0000000052,'faf','0167264242000000','','',0,'2014-04-14 11:48:42','2014-04-14 11:48:42'),(0000000053,'affadfa','9213472472482734','','adadasd',0,'2014-04-14 11:50:26','2014-04-14 11:51:58'),(0000000054,'NGUYỄN QUỐC ĐẠT','1','111111111131','232131312',1,'2014-04-14 11:59:28','2014-04-14 12:00:01'),(0000000055,'fsfsfs','tetggd353535','','',0,'2014-04-14 17:06:10','2014-04-14 17:06:10'),(0000000056,'fsfsfs','tetggd353535','','',0,'2014-04-14 17:21:28','2014-04-14 17:21:28'),(0000000057,'fsfsfs','tetggd353535','','',1,'2014-04-14 17:21:33','2014-04-14 17:21:33'),(0000000058,'a','tetggd353535','','',0,'2014-04-14 17:21:51','2014-04-14 17:21:51'),(0000000059,'a','dadadadadsdadada','','',1,'2014-04-14 17:22:31','2014-04-14 17:22:31'),(0000000060,'4424242','sffsdfsdfsdfsdfdfsdf','','',0,'2014-04-14 17:24:09','2014-04-14 17:24:09'),(0000000061,'thy','01672642420adadadas','okkkkkkkkk','',1,'2014-04-14 17:24:48','2014-04-15 01:36:42'),(0000000062,'thyyyyyyy','24242424242424342423','','',1,'2014-04-15 01:38:10','2014-04-15 01:38:10'),(0000000063,'Nguyen Cat Dut','82784823672682','me tri ha','nguyencatdut@gmail.com',1,'2014-04-15 01:46:34','2014-04-15 01:48:39'),(0000000064,'dadadasasdasdasafafsdf','1234567891232312','','fafa@fadsfa.co',0,'2014-04-15 02:24:26','2014-04-15 02:24:26');
/*!40000 ALTER TABLE `customer` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `order`
--

DROP TABLE IF EXISTS `order`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `order` (
  `OrderId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `OrderDate` datetime DEFAULT NULL,
  `Status` varchar(30) DEFAULT NULL,
  `Message` text,
  `CustomerId` int(10) unsigned zerofill NOT NULL,
  `CreatedAt` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`OrderId`),
  KEY `fk_Order_Customer1_idx` (`CustomerId`),
  CONSTRAINT `fk_Order_Customer1` FOREIGN KEY (`CustomerId`) REFERENCES `customer` (`CustomerId`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=27 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `order`
--

LOCK TABLES `order` WRITE;
/*!40000 ALTER TABLE `order` DISABLE KEYS */;
INSERT INTO `order` VALUES (1,'2014-03-11 00:00:00','Pending','asfafadf à ầ',0000000001,NULL),(2,'2014-03-10 00:00:00','Pending','afaf adfgsgs s',0000000001,NULL),(3,'2014-03-09 00:00:00','Shipped','Thank you',0000000003,NULL),(4,'2014-03-07 00:00:00','Canceled','Sản phẩm cùi mía',0000000004,NULL),(6,'2014-04-14 18:49:47','Pending','',0000000052,'2014-04-14 11:49:47'),(7,'2014-04-14 18:50:26','Pending','',0000000053,'2014-04-14 11:50:26'),(8,'2014-04-14 18:51:21','Pending','Sản phẩm rất hay, mua à nha',0000000053,'2014-04-14 11:51:21'),(9,'2014-04-14 18:51:31','Pending','Sản phẩm rất hay, mua à nha',0000000053,'2014-04-14 11:51:31'),(10,'2014-04-14 18:54:14','Pending','Sản phẩm rất hay, mua à nha',0000000001,'2014-04-14 11:54:14'),(11,'2014-04-14 18:54:52','Pending','Sản phẩm rất hay, mua à nha',0000000001,'2014-04-14 11:54:52'),(12,'2014-04-14 18:55:41','Pending','ĐƯỢC ĐẤY',0000000002,'2014-04-14 11:55:41'),(13,'2014-04-14 18:59:29','Pending','ĐƯỢC ĐẤY',0000000054,'2014-04-14 11:59:29'),(14,'2014-04-14 18:59:38','Pending','ĐƯỢC ĐẤY',0000000054,'2014-04-14 11:59:38'),(15,'2014-04-14 19:00:10','Pending','ĐƯỢC ĐẤY',0000000054,'2014-04-14 12:00:10'),(19,'2014-04-15 00:24:22','Pending','',0000000001,'2014-04-14 17:24:22'),(20,'2014-04-15 08:23:22','Pending','fuck you',0000000002,'2014-04-15 01:23:22'),(21,'2014-04-15 08:32:34','Pending','',0000000003,'2014-04-15 01:32:34'),(22,'2014-04-15 08:38:11','Pending','',0000000062,'2014-04-15 01:38:11'),(23,'2014-04-15 08:38:29','Pending','',0000000062,'2014-04-15 01:38:29'),(24,'2014-04-15 08:46:38','Pending','san pham kha la hay, toi se dat hang no hahahhahahhahah',0000000063,'2014-04-15 01:46:38'),(25,'2014-04-15 09:24:27','Pending','',0000000064,'2014-04-15 02:24:27'),(26,'2014-04-15 09:24:28','Pending','',0000000064,'2014-04-15 02:24:28');
/*!40000 ALTER TABLE `order` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `orderdetail`
--

DROP TABLE IF EXISTS `orderdetail`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `orderdetail` (
  `OrderDetailId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `OrderId` int(10) unsigned NOT NULL,
  `ProductId` int(10) unsigned NOT NULL,
  `Quantity` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`OrderDetailId`),
  KEY `fk_Order_has_Product_Product1_idx` (`ProductId`),
  KEY `fk_Order_has_Product_Order1_idx` (`OrderId`),
  CONSTRAINT `fk_Order_has_Product_Order1` FOREIGN KEY (`OrderId`) REFERENCES `order` (`OrderId`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_Order_has_Product_Product1` FOREIGN KEY (`ProductId`) REFERENCES `product` (`ProductId`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `orderdetail`
--

LOCK TABLES `orderdetail` WRITE;
/*!40000 ALTER TABLE `orderdetail` DISABLE KEYS */;
INSERT INTO `orderdetail` VALUES (1,1,1,2),(2,3,2,1);
/*!40000 ALTER TABLE `orderdetail` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `product`
--

DROP TABLE IF EXISTS `product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `product` (
  `ProductId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `ProductCode` varchar(45) DEFAULT NULL,
  `ProductName` varchar(90) DEFAULT NULL,
  `Brand` varchar(90) DEFAULT NULL,
  `Price` int(10) unsigned DEFAULT NULL,
  `QuantityInStock` int(10) unsigned DEFAULT NULL,
  `Description` text,
  `SaleOff` int(10) unsigned DEFAULT NULL,
  `TypeId` int(10) unsigned NOT NULL,
  PRIMARY KEY (`ProductId`),
  UNIQUE KEY `ProductId_UNIQUE` (`ProductId`),
  UNIQUE KEY `ProductCode_UNIQUE` (`ProductCode`),
  KEY `fk_Product_ProductType_idx` (`TypeId`),
  CONSTRAINT `fk_Product_ProductType` FOREIGN KEY (`TypeId`) REFERENCES `producttype` (`TypeId`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `product`
--

LOCK TABLES `product` WRITE;
/*!40000 ALTER TABLE `product` DISABLE KEYS */;
INSERT INTO `product` VALUES (1,'INT2201','Áo Thun Nam May 10','May 10',850000,20,'Áo Thun đẹp vãi đái',10,1),(2,'INT2202','Áo Thun Gucci','Gucci',1500000,15,'Gucci đẹp vãi đái',5,1);
/*!40000 ALTER TABLE `product` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `producttype`
--

DROP TABLE IF EXISTS `producttype`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `producttype` (
  `TypeId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `TypeName` varchar(90) DEFAULT NULL,
  `Gender` tinyint(1) DEFAULT NULL,
  `Kind` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`TypeId`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `producttype`
--

LOCK TABLES `producttype` WRITE;
/*!40000 ALTER TABLE `producttype` DISABLE KEYS */;
INSERT INTO `producttype` VALUES (1,'Đầm',0,'1'),(2,'Thun',1,'0');
/*!40000 ALTER TABLE `producttype` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-04-15  9:26:44
