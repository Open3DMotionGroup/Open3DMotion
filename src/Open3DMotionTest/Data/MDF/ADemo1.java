/*--
  Open3DMotion 
  Copyright (c) 2004-2012 Charnwood Dynamics Ltd.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

package com.codamotion.motionfile.test;

class ADemo1
{
	static final int nummarkers = 24;
	static final int numforceplates = 1;
	static final int numEMG = 8;
	static final double rateHz_marker = 200.0;
	static final double rateHz_force = 200.0;
	static final double rateHz_EMG = 400.0;
	static final int numframes_marker = 512;
	static final int numframes_force = 511;
	static final int numframes_EMG = 1023;
	static final double forcetol = 0.05;
	
	static final String markername[] =
	{
		"L.Sac.Wand",
		"R.Sac.Wand",
		"L.PSIS",
		"L.ASIS",
		"L.Post.Fem.",
		"L.Ant.Fem.",
		"L.Post.Tib.",
		"L.Ant.Tib.",
		"L.Knee",
		"L.Ankle",
		"L.Heel",
		"L.Toe",
		"R.PSIS",
		"R.ASIS",
		"R.Post.Fem.",
		"R.Ant.Fem.",
		"R.Post.Tib.",
		"R.Ant.Tib.",
		"R.Knee",
		"R.Ankle",
		"R.Heel",
		"R.Toe",
		"L.Hip",
		"R.Hip",
	};

	String EMGname[] =
	{
		"R.Quads",
		"R.Med.Hams",
		"R.Ant.Tib",
		"R.Gastroc",
		"L.Quads",
		"L.Med.Hams",
		"L.Ant.Tib",
		"L.Gastroc",
	};
	
	static final double forceplate0_outline[][] = {
		{ -300.0, -200.0, 0.0 },
		{ -300.0,  200.0, 0.0 },
		{  300.0,  200.0, 0.0 },
		{  300.0, -200.0, 0.0 }
	};
	
	static final String forceplate0_type = "Kistler";
	
	static final String subjectID = "hao011";
	
	static final double marker_50_frames[] = {
1907.9,-10.9,891.9,1,1898.1,22,889.4,1,1612.8,-164.4,845.4,1,1421.9,-155.2,815.4,1,1629,-135.8,446.3,1,1492.1,-115.2,386.1,1,1690,-132.7,358.3,1,1585.7,-177,260.5,1,1658,-116,376.9,1,1827.6,-81.6,58.9,1,1884.6,-64.1,46.3,1,1765.9,-109,-1.8,1,1609.8,174.9,837.4,1,1421.6,159.1,809.2,1,1346.5,143.5,425.1,1,1215.6,117.3,499,1,1341.7,140.6,354.1,1,1195,186.5,356,1,1327,127.8,377.9,1,1234.3,96.7,40.3,1,1272.9,80.5,-0.3,1,1150.7,114.5,27.2,1,1503.4,-172.6,742.6,1,1513.8,180,730.9,1, 
1907.9,-10.9,891.9,0,1898.1,22,889.4,0,1612.8,-164.4,845.4,0,1421.9,-155.2,815.4,0,1629,-135.8,446.3,0,1492.1,-115.2,386.1,0,1690,-132.7,358.3,0,1585.7,-177,260.5,0,1658,-116,376.9,0,1827.6,-81.6,58.9,0,1884.6,-64.1,46.3,0,1765.9,-109,-1.8,0,1609.8,174.9,837.4,0,1421.6,159.1,809.2,0,1346.5,143.5,425.1,0,1215.6,117.3,499,0,1341.7,140.6,354.1,0,1195,186.5,356,0,1327,127.8,377.9,0,1234.3,96.7,40.3,0,1272.9,80.5,-0.3,0,1150.7,114.5,27.2,0,1503.4,-172.6,742.6,0,1513.8,180,730.9,0, 
1893.5,-6.8,892.8,1,1885.7,18.3,890.2,1,1598.4,-162.3,843.6,1,1407.9,-153.1,812.3,1,1616.3,-134.9,444.7,1,1479.7,-114.4,383.7,1,1678.5,-131.9,358.2,1,1575.9,-175.1,257.9,1,1646,-115.3,376,1,1825.2,-81.5,63.9,1,1881.4,-66.5,52.6,1,1766.6,-107.1,-2.3,1,1593.9,175,837.6,1,1405.2,159.9,807.8,1,1328.3,143.5,425.3,1,1197.4,116.6,500.2,1,1323.4,143.2,354,1,1177.7,185.6,349.5,1,1308.9,127.9,377.1,1,1231.9,93.2,36.3,1,1273.6,78.9,0.4,1,1148.8,112.8,17,1,1489.8,-168.6,741.3,1,1498.5,180.1,730.4,1, 
1885.4,-7,893,1,1879.5,15.3,890.3,1,1590.8,-161.6,842.7,1,1399.6,-153.9,810.7,1,1609.4,-134.8,444.2,1,1473.5,-112.1,382.4,1,1671.8,-132.9,358.3,1,1571,-174.6,256.7,1,1639.9,-114.5,375.6,1,1823,-83,67.8,1,1880.5,-65.8,57,1,1766.3,-107.2,-2.4,1,1586.3,175.6,837.9,1,1398,159.8,807.5,1,1319.2,143.9,425.8,1,1188.2,115.9,500.6,1,1315.4,141.2,354.3,1,1170,183.7,345.5,1,1300.3,126.8,377.6,1,1230.4,94.2,35.5,1,1273.2,79.1,1.1,1,1147.9,111.8,14.1,1,1482.7,-166.7,740.6,1,1490.5,181.4,730.6,1, 
1877.9,-5.3,893.1,1,1872.8,14.2,890.2,1,1583.9,-159.2,841.9,1,1392.3,-152.8,809.8,1,1602.4,-134,443.6,1,1466.5,-110.8,381.4,1,1666.5,-130.3,358.4,1,1566.4,-173.2,255.6,1,1633.5,-113.6,374.9,1,1823,-83,67.8,0,1878,-67.7,61.4,1,1765.5,-107.9,-1.7,1,1579.4,174.3,838.5,1,1390.6,160.3,808,1,1309.4,146.2,426.9,1,1178.4,114.7,501.5,1,1307.3,140.9,354.6,1,1161.4,184.8,342,1,1291.5,126.2,378.4,1,1228.8,93.6,34.8,1,1272.7,77.2,1.5,1,1147,109.2,11.8,1,1474.8,-165.7,740,1,1483.2,180.4,730.7,1, 
1870.3,-4.7,892.9,1,1864.3,16.2,890.1,1,1576.2,-158.6,841,1,1384.5,-152,809.1,1,1595.1,-133.3,443,1,1459.1,-109.9,380.7,1,1659.3,-131.8,358.4,1,1561.2,-173.7,254.2,1,1627.4,-111.6,374.4,1,1823,-83,67.8,0,1876.3,-67.5,66.9,1,1764.8,-109,-1.2,1,1573.1,172.6,839.1,1,1382.9,161.9,808.2,1,1301.1,143.5,427.8,1,1169.5,112.7,501.8,1,1298.6,142,356.1,1,1153.7,183.5,339.2,1,1282.8,126.3,379.1,1,1226.3,94.2,35.6,1,1271.3,78.5,2.7,1,1145.1,110.1,10.9,1,1466.7,-164.9,739.8,1,1475.1,181.1,731.3,1, 
1862.8,-4.9,892.3,1,1858.3,13,889.6,1,1568.6,-157.7,840.3,1,1376.4,-151.9,808.7,1,1587,-133.4,442.7,1,1451.5,-108.7,379.7,1,1652.1,-131.8,358.8,1,1556.2,-173.4,253.2,1,1619.5,-112.9,374.3,1,1823,-83,67.8,0,1874.8,-66.2,72.8,1,1765.1,-107.6,-1.1,1,1565.5,173.7,839.9,1,1376.2,161.9,809.5,1,1291.6,144,429.5,1,1160.1,109.1,503,1,1290.5,141.5,357.1,1,1145.9,182.5,338.2,1,1274.2,126.2,380.6,1,1224.7,93,35.8,1,1270,80.3,3.3,1,1144.2,110.2,8.8,1,1458.5,-164.3,739.2,1,1466.9,181.9,732.3,1, 
1854.2,-7.1,891.3,1,1850.3,15,889,1,1561.3,-155.7,839.5,1,1367.1,-154.1,808.5,1,1579.5,-131.8,442.1,1,1443.2,-108.8,379.6,1,1645,-131.6,359.1,1,1550.1,-175.9,252.4,1,1612.4,-111.6,373.9,1,1823,-83,67.8,0,1870.8,-69.5,81.6,1,1764.9,-107.1,-0.6,1,1558.6,173.6,841.2,1,1369.8,160.8,811,1,1282.6,143.5,431.1,1,1150.8,108.1,504.3,1,1282.9,141.5,358.5,1,1138.4,180,337.5,1,1266.1,125.9,381.6,1,1222.8,94.7,36.3,1,1270,79.5,3.4,1,1143.7,109.2,7.2,1,1450.5,-163,738.6,1,1459.8,180.7,733.4,1, 
1846.1,-9.1,890.1,1,1843.9,12.1,888,1,1553.1,-155.7,838.9,1,1359.1,-152.9,808.4,1,1571.4,-130.1,441.5,1,1434.1,-109.8,379.2,1,1638.2,-129.8,359.1,1,1544.6,-176.1,251.4,1,1604.7,-110.7,373.6,1,1823,-83,67.8,0,1870.8,-69.5,81.6,0,1764.8,-106.5,0,1,1551.4,174.4,842.3,1,1363.4,159.2,812.5,1,1273.8,142.9,432.9,1,1142.4,105.4,506,1,1276.4,139.1,359.7,1,1131.6,177.7,338.1,1,1258.8,124.4,382.6,1,1222.4,93.5,35.8,1,1269.2,81,4.4,1,1143.2,110,6.4,1,1442.1,-162.3,738.2,1,1452.1,181.1,734.7,1, 
1839.6,-8.6,889.3,1,1836.9,10.1,887,1,1545.4,-154.8,838.1,1,1351,-151.4,808.4,1,1562.1,-130.3,441,1,1424.3,-110.9,379.2,1,1630.6,-128.2,359,1,1538.5,-177.4,250.6,1,1596.5,-109.9,373,1,1823,-83,67.8,0,1870.8,-69.5,81.6,0,1763.3,-107.8,0.8,1,1544.9,173,843.7,1,1355.7,161.1,814.4,1,1264.5,144.1,434.6,1,1133.8,102.1,507.9,1,1269.2,139.5,360.9,1,1124.6,174.2,339.3,1,1251.4,125.5,383.4,1,1221.4,93.7,35.2,1,1268.7,80.5,4.6,1,1142.7,109.8,6.1,1,1433.8,-161.5,737.8,1,1445.1,179.9,736.1,1, 
1832.3,-9.9,888.4,1,1829,10.3,886.1,1,1537.4,-154.3,837.4,1,1342.7,-150.4,808.6,1,1552.9,-129.5,440.4,1,1414.4,-111.2,379.1,1,1623,-126.4,359.3,1,1531.8,-179.5,249.8,1,1587.1,-111.3,372.7,1,1823,-83,67.8,0,1870.8,-69.5,81.6,0,1763.3,-106.8,1.4,1,1537.6,173,844.8,1,1348.6,162.3,816.5,1,1256.3,143.1,435.8,1,1127,95.7,509.5,1,1262,140,362.2,1,1116.8,172.9,341.1,1,1245.2,124.2,384,1,1220.5,92.6,34.8,1,1268.6,79.4,4.6,1,1142.1,110.9,5.9,1,1425.7,-160.3,737.2,1,1437.2,180.5,737.9,1, 
1823.8,-14,887,1,1823,5.2,885.2,1,1529.1,-154.2,836.7,1,1334.5,-149.3,808.4,1,1542.8,-129.6,439.8,1,1404.3,-110.6,378.7,1,1613.1,-128.3,359.7,1,1524.8,-181.1,249.1,1,1578.3,-110.3,372,1,1823,-83,67.8,0,1870.8,-69.5,81.6,0,1762.3,-106.8,2.4,1,1530.9,172.3,845.9,1,1342.4,160.4,818.4,1,1249,141.5,436.8,1,1120.5,92.9,510.8,1,1255.6,138.9,362.6,1,1110.4,167.9,341.3,1,1238.7,123.5,384.2,1,1218.9,92.9,35.2,1,1266.9,83,5.7,1,1141.8,110.9,5.8,1,1417.5,-158.8,736.6,1,1428.8,182.9,739.7,1, 
1817.4,-13.9,885.8,1,1815.6,2.6,884.5,1,1521.8,-151.9,836.2,1,1326.4,-147.8,808.8,1,1532.7,-129.3,438.8,1,1392.8,-112.7,379.2,1,1604.6,-126.5,359.5,1,1518,-182.5,248.4,1,1568,-111.8,371.7,1,1823,-83,67.8,0,1870.8,-69.5,81.6,0,1760.9,-107.4,3.9,1,1524.1,171.9,847.4,1,1334.1,164.9,820.5,1,1241.3,142.3,438,1,1114.1,88.9,511.5,1,1249,139.3,363.2,1,1103,165,341.1,1,1232.4,123.3,384.4,1,1217.9,93.1,35.3,1,1267.8,79.8,5.2,1,1141.8,110.5,5.6,1,1408.8,-158.9,736.2,1,1420.9,183.7,741.3,1, 
1808.5,-18.6,884.9,1,1808.3,-0.1,883.4,1,1513.2,-153,835.6,1,1318.1,-146.6,809,1,1522.7,-127.9,437.5,1,1381.6,-113.1,379.2,1,1593.2,-129.6,359.9,1,1510.9,-183.5,247.3,1,1557.7,-112.2,370.9,1,1823,-83,67.8,0,1870.8,-69.5,81.6,0,1759.7,-107,5,1,1516.9,172.7,848.6,1,1327.7,164.3,822.4,1,1234.4,141.4,438.3,1,1107.9,87.8,511.5,1,1242.7,138.9,363.6,1,1096.4,160.2,339.3,1,1226.3,122.5,384.6,1,1217.9,91.9,35.2,1,1267.1,80.6,6,1,1141.7,109.5,5.7,1,1400.3,-158.1,735.9,1,1413.8,182.5,743.3,1, 
1801.5,-19.9,883.7,1,1801,-2.2,882.5,1,1506,-150.5,835.3,1,1310.4,-144.4,809.6,1,1511,-129.7,436.6,1,1368.9,-116.6,379.4,1,1584.2,-127.2,359.4,1,1503.2,-185.5,246.6,1,1547.4,-111.8,370,1,1823,-83,67.8,0,1870.8,-69.5,81.6,0,1757.8,-107.8,6.6,1,1510.2,172.8,849.8,1,1319.5,168.7,824.5,1,1228.1,140.7,438.9,1,1102.3,83.7,510.8,1,1236.4,138.8,363.6,1,1089.9,157.4,336.7,1,1219.9,122.6,384.6,1,1217.5,91.9,35.3,1,1266.7,81.6,6.3,1,1141.1,111.1,5.9,1,1391.7,-157.6,735.4,1,1405.6,183.6,745.2,1, 
1793.3,-22.6,882.9,1,1793.4,-3.4,881.3,1,1497.4,-151.6,834.9,1,1301.5,-145.7,809.8,1,1499.9,-129.8,435.2,1,1357,-117.7,379.7,1,1573.9,-126.3,359.5,1,1495.3,-187.8,245.9,1,1536.5,-112.4,369,1,1823,-83,67.8,0,1870.8,-69.5,81.6,0,1756.5,-107.2,7.7,1,1503.8,171.9,851.2,1,1312.5,169.8,826.7,1,1221.8,140.5,438.5,1,1096.9,77.9,509.7,1,1230.6,138.3,363.3,1,1084.1,153.6,333.6,1,1214.6,120.4,384.2,1,1216.8,92.5,35.5,1,1266.1,82.9,6.5,1,1141.5,110,5.6,1,1383.7,-155.8,735.5,1,1397.6,184,747.3,1, 
1785.1,-25.2,882.3,1,1786.9,-5.5,881.2,1,1489.8,-150.5,834.6,1,1293.7,-144.3,810.5,1,1488.7,-129.8,434,1,1344.9,-119.8,379.7,1,1561.5,-129.3,359.4,1,1486.5,-190.8,245.2,1,1525.7,-111.3,367.8,1,1823,-83,67.8,0,1870.8,-69.5,81.6,0,1754.9,-107,9.5,1,1496,174.1,852.4,1,1305.6,170.7,828.6,1,1216.3,139.3,438,1,1090.8,75.5,507.2,1,1225,138.9,363.3,1,1079.1,150,329.8,1,1209,119.4,383.9,1,1217,91.4,35.3,1,1267.1,80,6.2,1,1141,111.3,5.9,1,1375,-155.6,735.3,1,1389.6,184.9,749.3,1, 
1778.2,-24.1,881.7,1,1778,-2.1,880.4,1,1481.9,-150.9,834.6,1,1286,-143.7,811.3,1,1476.8,-130.7,432.9,1,1332.9,-121.3,380.1,1,1550.6,-128.3,359.2,1,1478,-192.6,244.5,1,1514.3,-110.7,366.4,1,1823,-83,67.8,0,1870.8,-69.5,81.6,0,1753.5,-105.9,10.8,1,1490.3,171.3,853.5,1,1298.3,172.2,831.1,1,1211.6,137.3,437.2,1,1085,73.1,504.2,1,1220.2,137.7,362.6,1,1075,146.3,326.4,1,1203.4,120.8,383.8,1,1216.3,92.3,35.5,1,1266.4,81.4,6.6,1,1141.1,111.3,6,1,1366.7,-155.1,735,1,1382.1,185.1,750.8,1, 
1770.4,-25.1,881.5,1,1771.4,-2.9,880.6,1,1475,-149.4,834.8,1,1278.9,-142.7,812.2,1,1465.1,-131.4,431.7,1,1321.2,-122.6,380.2,1,1538.8,-128.8,358.3,1,1468.9,-194.8,243.8,1,1501.8,-112,365.5,1,1823,-83,67.8,0,1870.8,-69.5,81.6,0,1750.9,-106.7,13.4,1,1483.2,171.6,854.8,1,1291,173.1,833.2,1,1206.5,137.6,436.5,1,1079.1,71.9,501.2,1,1215.7,138,362.4,1,1071.1,144.2,323.2,1,1199.4,118.1,383.3,1,1216.5,91.6,35.3,1,1266,82.9,6.7,1,1141.5,109.6,5.6,1,1358.6,-154,734.8,1,1374.6,185.2,752.7,1, 
1764.4,-21.7,881.7,1,1764.1,-1.1,880.8,1,1467,-150.6,834.9,1,1271.1,-143.9,812.8,1,1454,-130,430.2,1,1308.4,-126,381,1,1527.2,-128.2,358.2,1,1458.7,-197.6,242.7,1,1489.5,-111.7,363.9,1,1823,-83,67.8,0,1870.8,-69.5,81.6,0,1747.9,-106.8,16.6,1,1475.6,173,856,1,1283.8,174.2,835.2,1,1201.3,138.9,435.8,1,1072.8,72.9,497.5,1,1211.9,136.6,361.9,1,1067.3,143.3,320.7,1,1194.9,118.9,383.1,1,1216.7,90.7,35,1,1266.5,82,6.7,1,1141.1,111,5.8,1,1351.4,-151.6,734.8,1,1367.9,183.8,754.4,1, 
1757.6,-20.4,881.8,1,1758.1,-1.3,881.1,1,1460.6,-149.5,835.2,1,1264.9,-142.1,813.6,1,1442.3,-130,428.9,1,1296.6,-126.6,381.4,1,1516,-125.5,357.1,1,1448.6,-198.5,241.4,1,1476,-113.5,362.8,1,1823,-83,67.8,0,1870.8,-69.5,81.6,0,1744.7,-105.2,20.8,1,1468.9,172.5,857.3,1,1276.5,174.9,837.2,1,1197.2,138.5,435.2,1,1067.9,71.5,494.8,1,1208.3,136.4,361.7,1,1064.1,144.1,319.5,1,1190.9,120.4,383,1,1216.1,92.3,35.2,1,1266.6,81.8,6.6,1,1141.6,109.7,5.6,1,1343.5,-151.5,734.7,1,1361.1,182.8,756.5,1, 
1750.1,-19.9,882.4,1,1751.1,0.3,881.9,1,1453.8,-149.2,835.6,1,1258.4,-141.8,814.3,1,1430.4,-130.4,427.5,1,1284.2,-128.9,382.4,1,1503.3,-126.1,356.4,1,1437.8,-199.5,239.6,1,1463.1,-113.6,361.3,1,1823,-83,67.8,0,1870.8,-69.5,81.6,0,1738.7,-106.8,26.4,1,1461.4,173.8,859.1,1,1269.4,174.8,839.1,1,1192.8,140.3,435,1,1062.9,72.9,492.7,1,1204.9,137.2,361.9,1,1061.7,142.9,318,1,1188.6,117.6,382.4,1,1216.2,91.8,35.5,1,1266.1,83.1,6.6,1,1141.7,109.5,5.7,1,1336,-151.4,735,1,1354.5,181.7,758.5,1, 
1744.2,-16.8,883.1,1,1743.8,3.1,882.4,1,1447.3,-148.9,835.7,1,1252.2,-141.1,814.9,1,1418.1,-131.4,426.5,1,1271,-131.7,383.8,1,1489.7,-127.7,356,1,1426.2,-200.3,237.7,1,1449.6,-114.3,360,1,1823,-83,67.8,0,1870.8,-69.5,81.6,0,1733,-106.2,32,1,1454.3,173.7,860.8,1,1261.9,175.4,840.6,1,1189.7,139,434.7,1,1058.8,74.6,490.5,1,1202.6,136.1,361.5,1,1059.3,144.4,317.5,1,1185.4,118.9,382.3,1,1215.8,93.2,35.6,1,1267.6,78.6,6,1,1141.6,110,5.9,1,1328.7,-150.8,735.1,1,1347.5,182.2,760.7,1, 
1738.4,-13.4,884.3,1,1737.9,3.6,883.4,1,1441.6,-147.1,836,1,1245.9,-141.7,815.7,1,1405,-133.9,425.6,1,1258.5,-131.6,385.1,1,1476.7,-127.1,354.9,1,1415.1,-198.1,235.5,1,1435.9,-115.3,358.9,1,1823,-83,67.8,0,1870.8,-69.5,81.6,0,1725.3,-107.3,38.4,1,1446.7,174.9,862.4,1,1254.3,176.3,842.6,1,1186.2,140.6,434.2,1,1055.1,74.4,489.6,1,1199.8,138.1,361.7,1,1056.9,147.5,317.3,1,1183.4,118.2,381.8,1,1215.9,93,35.4,1,1267,80.9,6.5,1,1141.5,110.2,5.6,1,1322.4,-148.8,735.3,1,1340.5,183.4,762.6,1, 
1732.3,-11.3,885.6,1,1730.4,7.8,884.7,1,1435.7,-146.7,836.3,1,1239.4,-143.4,815.8,1,1393.2,-133.2,424.4,1,1245.7,-131.5,386.7,1,1462.6,-129.1,354.4,1,1402.2,-198.5,233.6,1,1421.9,-116.7,358,1,1823,-83,67.8,0,1732.5,-88.2,194.8,1,1717.7,-106.9,45.2,1,1440.2,173.5,864.7,1,1246.6,177.5,843.9,1,1183.6,139.7,434.1,1,1052.2,75.3,488.8,1,1197.9,138.1,361.6,1,1055.7,147.7,317.1,1,1180.9,119.7,381.6,1,1216.4,91.9,35,1,1266.9,81.7,6.1,1,1141.6,110.8,5.6,1,1315.7,-148,735.6,1,1333.6,184.3,764.6,1, 
1725,-11.1,887,1,1724.3,8.9,885.7,1,1430.2,-145.6,836.6,1,1233,-145.3,816,1,1380.9,-132.8,423.5,1,1232.7,-130.7,388.7,1,1449.3,-128.6,353.7,1,1388.2,-199.5,232.3,1,1407.9,-118.3,357.4,1,1823,-83,67.8,0,1722.5,-84.4,199.1,1,1708.6,-106.6,51.3,1,1432.2,176.2,866.4,1,1240.1,175,845.7,1,1181.2,139.5,434.2,1,1049.2,77.2,488.1,1,1196.1,139.4,361.6,1,1054.2,150.9,316.3,1,1179,120.2,381.3,1,1216.4,92.5,34.8,1,1267.5,79.7,6,1,1141.2,112.2,5.8,1,1309.3,-147,736.2,1,1326.7,185.6,766.5,1, 
1719.5,-7.8,888.3,1,1717.8,11,887.1,1,1424.6,-144.7,836.9,1,1228.5,-142.5,816.5,1,1367.8,-134.5,422.9,1,1219.6,-129.7,391,1,1435,-130.7,353.5,1,1373.8,-199.9,230.9,1,1394.3,-118.2,356.9,1,1823,-83,67.8,0,1711.4,-84.1,202.8,1,1698.2,-107,56.7,1,1425.4,174.9,868.5,1,1232.1,177.1,847.1,1,1178.7,140.2,434.2,1,1046.1,80.5,487.6,1,1194.8,138.6,361.6,1,1053.2,153.3,315.1,1,1177.3,120.2,380.9,1,1216.6,91.4,35.1,1,1267.5,80.5,6.2,1,1141.6,111.2,5.8,1,1302.2,-148.6,736.9,1,1319.4,187.8,768,1, 
1713.2,-6.1,890.2,1,1712.2,11.5,888.9,1,1418.4,-145.9,837.4,1,1223.4,-141,816.8,1,1354.5,-135.9,422.5,1,1206.8,-127.5,393,1,1420.4,-132.7,353.6,1,1359.9,-197.7,229.7,1,1380.1,-119.4,356.8,1,1823,-83,67.8,0,1701.3,-81.6,205.7,1,1686.5,-106.8,61.5,1,1418.4,174.9,870.3,1,1225.3,175.4,848.1,1,1176.4,141.1,434.1,1,1043,82.2,486.8,1,1193.1,140.2,361.3,1,1052.6,155.7,313.7,1,1175.2,122.4,380.9,1,1216.4,92.4,35.2,1,1267.2,81.5,5.9,1,1141.5,111.5,5.6,1,1296.5,-146.6,737.7,1,1312.9,188.3,769.3,1, 
1706.5,-5.1,891.8,1,1705.9,13.6,890.5,1,1413.5,-144.1,837.7,1,1217.6,-141.9,817.1,1,1341.9,-135.9,422.1,1,1194.3,-124.8,395.2,1,1406.2,-133.8,353.9,1,1344.9,-197.2,229.1,1,1365.9,-120.3,357.1,1,1823,-83,67.8,0,1690.2,-79.9,208,1,1672.9,-107.8,65.7,1,1410.7,177,872.1,1,1218,175.2,849.2,1,1174.4,141.2,434.1,1,1039.7,85.7,485.3,1,1191.4,141.9,361.1,1,1051.7,159.4,312,1,1173.6,122.7,380.7,1,1216.7,91.7,34.6,1,1265.8,85.7,6.9,1,1142,110.3,5.1,1,1290.3,-146.5,738.5,1,1307.1,186.7,771,1, 
1699.3,-4.9,893.8,1,1699.8,15.6,892.1,1,1408,-143.6,838.3,1,1212.3,-142.1,817.5,1,1329.3,-135.7,422.1,1,1181.4,-123.8,397.3,1,1391.5,-134.2,354.5,1,1330.3,-195.6,229,1,1351.9,-120.4,357.6,1,1823,-83,67.8,0,1677.7,-80.2,210.6,1,1658.2,-109.5,71,1,1403.4,177.1,873.8,1,1210.9,174.9,850.4,1,1172.9,139.6,433.4,1,1037.1,90.5,483.6,1,1190.5,141.5,361.1,1,1051.9,160.5,309.7,1,1172.4,121.4,380.1,1,1216.3,93.2,35,1,1267.6,79.2,5.3,1,1141.8,111.1,5.3,1,1284.4,-146.1,739.5,1,1300.2,188.4,772.2,1, 
1694.9,0.4,895.9,1,1693.3,17.9,894.2,1,1402.1,-144.3,838.9,1,1207.2,-141.6,817.7,1,1317,-134.4,422.3,1,1167.9,-124.3,399.8,1,1377.6,-133.3,355.1,1,1315.9,-193.4,228.9,1,1337.7,-121.1,358.4,1,1823,-83,67.8,0,1665.1,-79.4,212.4,1,1644,-108.8,74.4,1,1396.3,178.3,875.2,1,1204.3,173.8,851.6,1,1170.3,142.2,433.7,1,1033.5,91.1,481.9,1,1189.2,142.5,360.6,1,1051.5,163.6,307.5,1,1170.4,123.1,379.8,1,1216.4,92.6,35,1,1266.7,81.5,6.6,1,1142.1,110.6,5.2,1,1278.7,-146,740.8,1,1294.2,188.4,773.5,1, 
1689.3,4,897.7,1,1688.2,17.1,896.1,1,1396.7,-143.7,839.8,1,1202.5,-139.6,818.2,1,1304,-134.9,422.9,1,1155.2,-123.9,402,1,1362.8,-134.5,355.9,1,1301.9,-190.9,229.1,1,1324.4,-119,359.2,1,1823,-83,67.8,0,1654.1,-74.2,213.4,1,1629.2,-108.5,78,1,1389.1,179.6,876.2,1,1197.4,173.3,852.7,1,1168.9,141.8,433.6,1,1030.8,93.2,479.6,1,1188.2,142.6,360.5,1,1051.5,165.9,305.5,1,1169.1,123.1,379.3,1,1216.3,92.9,35.2,1,1267.3,81.4,5.8,1,1142.2,109.7,4.8,1,1272.9,-146.3,741.7,1,1287.7,189.8,774.4,1, 
1683.4,6.6,899.8,1,1682.4,18.8,897.3,1,1391.2,-143.4,840.8,1,1197.1,-139.9,818.5,1,1291.1,-134.8,423.9,1,1142,-125.3,404.9,1,1347.7,-136.2,357,1,1287.2,-190.1,229.9,1,1309.8,-120.4,360.3,1,1823,-83,67.8,0,1640.8,-73.1,214.2,1,1613.3,-110.3,81.6,1,1383.2,177.3,877.7,1,1190.8,172.7,853.4,1,1166.6,143.5,433.2,1,1027.5,95.7,477.5,1,1186.8,143.5,360,1,1050.5,170.6,303.8,1,1167.2,124.4,379,1,1216.8,91.5,34.6,1,1266.8,82.7,5.9,1,1142.1,110.8,5,1,1267.4,-145.7,743,1,1281.5,190.7,775.7,1, 
1678.4,11,901.6,1,1676,21.1,899.4,1,1385.9,-142.5,841.5,1,1192.5,-137.8,819.2,1,1278.5,-134.9,425.1,1,1129.5,-125.9,407.8,1,1333.5,-135.8,357.9,1,1272.9,-189.1,231.1,1,1295.8,-120.2,361.5,1,1584.6,-88.3,186.6,1,1627.4,-72.3,215.1,1,1598,-110,84.3,1,1375.7,179.4,878.7,1,1184,172.8,854.3,1,1165.2,142.5,432.8,1,1024.9,98.4,475.5,1,1185.6,144.3,359.9,1,1050.3,172.2,302.2,1,1165.7,124.8,378.6,1,1216.6,91.9,34.5,1,1266.8,82.9,6.3,1,1142,111.5,5.1,1,1262.2,-144.8,744.4,1,1275.8,190.2,776.7,1, 
1671.7,12,903.4,1,1670.2,22.6,901.4,1,1380.8,-140.8,842.8,1,1186.5,-139.2,820,1,1265.8,-134.7,426.5,1,1116.7,-127.6,411.3,1,1320.3,-133.4,358.9,1,1258.7,-188,232.3,1,1282.1,-119.4,362.8,1,1570.4,-87.6,186.7,1,1613.7,-71.8,215.2,1,1582.1,-109.8,84.8,1,1369.5,178.7,880,1,1177,174,854.9,1,1163.7,141.7,432.7,1,1022.5,100.3,473.8,1,1184.2,145,359.6,1,1050.4,171,300.7,1,1164.3,125.2,378.3,1,1216.7,91.9,34.3,1,1266.6,83.3,6.3,1,1141.9,111.9,4.9,1,1256.7,-144.2,745.7,1,1269.7,191.5,777.6,1, 
1665.7,14.4,905.5,1,1664.7,22.8,903.1,1,1374.8,-141.6,843.8,1,1180.3,-140.9,820.8,1,1253.4,-134.2,427.9,1,1103.9,-129.6,415.1,1,1306.1,-133.5,360,1,1244.4,-187.3,234.1,1,1268,-119.4,364.3,1,1556.3,-86.1,186.8,1,1600.5,-69.8,214.3,1,1565.7,-109.2,85,1,1363.1,179.2,880.9,1,1170.5,174,855.5,1,1161.5,142.8,432.6,1,1019.9,101.8,471.9,1,1183.3,144,359.1,1,1049,174,299.7,1,1162.9,125.3,378,1,1216.4,92.9,34.8,1,1266.7,84,6.6,1,1141.9,111.8,5,1,1251.6,-143.2,746.8,1,1264.5,190.2,778.5,1, 
1660.5,17.8,907.1,1,1657.5,26.6,904.8,1,1369.4,-140.1,845.1,1,1174.7,-140.7,822.1,1,1241.2,-133.9,429.7,1,1091.5,-130.1,419.1,1,1292.2,-132.8,361.3,1,1230.7,-185.5,235.4,1,1254.3,-118.8,365.8,1,1542.9,-82.6,186.4,1,1587.4,-68.1,213.4,1,1549.2,-108,85.6,1,1357.5,177.4,881.9,1,1164.1,174.1,856.1,1,1159.4,143.9,432.2,1,1017.5,101.4,470.6,1,1181.6,144.8,359,1,1048.4,172.6,298.9,1,1161.7,124.8,377.5,1,1216.2,93.2,34.5,1,1266.6,83.9,6.9,1,1142.3,110.5,4.5,1,1246,-143.6,748.3,1,1258.6,190.9,779.5,1, 
1654.7,19.7,908.7,1,1652.5,25.2,906.2,1,1363,-141.5,846.5,1,1169.3,-139.5,823,1,1228.8,-133.9,431.4,1,1078.7,-131.4,423.2,1,1278.2,-133.1,362.9,1,1215.8,-186.7,237.6,1,1240.9,-117.9,367.4,1,1529,-79.9,185.7,1,1573.2,-67.2,212.1,1,1532.5,-106.3,85.8,1,1350.2,180.4,882.6,1,1158,173.9,856.5,1,1157.8,142.6,431.8,1,1015.6,102.1,469.2,1,1180.3,144.9,359.1,1,1046.7,173.5,298.3,1,1160,125.7,377.4,1,1216.9,91.7,34.2,1,1266.6,84.6,6.6,1,1142.1,111.4,4.7,1,1240.4,-143.3,749.6,1,1252.8,191.9,780.2,1, 
1649,21.9,910.6,1,1647.4,24.1,907.6,1,1357.3,-140.8,848.1,1,1163.9,-138.1,824.4,1,1216.5,-133.9,433.2,1,1066.8,-130.2,427.3,1,1265.5,-129.8,364.2,1,1201.3,-186.2,239.6,1,1227.6,-116.5,369.2,1,1513.6,-79.6,184.6,1,1559.6,-65.8,209.5,1,1515.5,-104.2,83.9,1,1344.2,180.6,883.2,1,1151.9,173.8,857.5,1,1155.8,143.3,431.7,1,1013.4,100.8,467.7,1,1179,144.4,358.9,1,1045.7,171,297.6,1,1158.7,125.3,376.9,1,1216.7,91.8,34.3,1,1266.8,84,6.6,1,1142,111.7,4.7,1,1234.4,-144.3,751.1,1,1247,192.4,781,1, 
1642.5,21.9,912.1,1,1641,25.8,909.3,1,1350.7,-141.8,849.6,1,1158.3,-136.6,825.9,1,1204.6,-132.9,434.9,1,1054.9,-128.8,431.5,1,1251.9,-129.6,365.8,1,1186.6,-186,241.7,1,1213.6,-117.5,371.3,1,1497.5,-80.3,183.6,1,1545.1,-64.7,207.1,1,1497.1,-104.3,83.1,1,1337.6,182.2,883.6,1,1146.1,172.5,857.9,1,1154.3,141.5,431.6,1,1011.5,103.2,466.3,1,1177.6,144.4,358.8,1,1043.6,170.9,297.2,1,1157.4,124.6,377,1,1216.3,92.9,34.5,1,1267.2,83.1,6.7,1,1142,111.8,4.6,1,1228.9,-143.6,752.4,1,1241.2,192.7,781.6,1, 
1636.7,23.1,913.6,1,1636.3,22.9,910.7,1,1344.9,-141,851.2,1,1152.3,-136.3,827.4,1,1193.1,-131,436.7,1,1041.3,-132.8,436.5,1,1238.5,-129.3,367.5,1,1172.5,-183.8,244,1,1201.2,-114.4,373,1,1481.6,-79.3,181.9,1,1530.8,-63,204.5,1,1477.5,-106.2,82.5,1,1331.5,182.7,884.2,1,1139.9,173.4,858.4,1,1151.9,143,431.3,1,1009,103.3,464.7,1,1175.7,145.2,358.8,1,1042,169,296.4,1,1155.7,125.6,376.8,1,1216.2,93,34.7,1,1267.8,81.2,6.9,1,1141.7,112.8,4.9,1,1223,-143.2,753.9,1,1235.9,191.8,782.4,1, 
1631.6,25.8,914.9,1,1629.7,24.2,911.7,1,1338.9,-140.2,852.8,1,1145.7,-137.8,828.8,1,1181.2,-130.2,438.7,1,1029.6,-130.2,440.5,1,1225.3,-128.2,369,1,1157.1,-185,246.7,1,1188,-114.1,374.9,1,1465.8,-79,180,1,1516.2,-60.7,200.8,1,1458.8,-105.3,81.1,1,1325.3,184,884.9,1,1134.7,171.4,858.8,1,1150,142.5,431.1,1,1006.9,101,463,1,1174.6,143.8,358.6,1,1040,167.9,296.3,1,1154.6,124.8,376.7,1,1216.6,91.6,34.5,1,1266.7,83.8,7,1,1141.6,113.6,5.1,1,1217.2,-142.9,755.1,1,1230.4,191,782.8,1, 
1624.7,24.4,916.3,1,1624,23.9,912.8,1,1332.7,-140.2,854.3,1,1140.1,-135.7,830.3,1,1169.3,-130.1,440.6,1,1017.3,-130.6,445.2,1,1211.9,-128,370.9,1,1142.1,-184.8,249.2,1,1174.7,-114.1,377,1,1448.5,-80.6,178.2,1,1499.8,-62.2,197.9,1,1438.9,-107.3,79.3,1,1318.9,185.9,885.3,1,1129.2,170.6,859.1,1,1148.2,141.6,430.9,1,1004.9,101.9,460.8,1,1173.3,143.3,358.6,1,1038,167.4,296.1,1,1152.9,125.6,376.8,1,1215.9,93.7,34.9,1,1267.2,82.4,6.7,1,1142.2,111.3,4.5,1,1210.9,-143.2,756.2,1,1223.9,193.5,783.3,1, 
1619.1,25.8,917.6,1,1618.1,23.3,913.9,1,1326.6,-139.5,855.9,1,1134.5,-133.8,831.7,1,1157.8,-128.7,442.5,1,1005.6,-129.1,449.6,1,1199,-126.4,372.5,1,1127.5,-183.2,251.9,1,1162.1,-112.3,379,1,1431.5,-82,175.9,1,1484.3,-61.3,193.8,1,1418.6,-109.4,78.3,1,1313.7,183.8,885.9,1,1123.3,171.4,859.6,1,1146.2,141.9,430.6,1,1002.8,101.2,459.2,1,1171.4,144.4,358.6,1,1036.6,165,295.3,1,1151.8,124.2,376.4,1,1216.2,92.2,34.5,1,1267.9,80.5,6.9,1,1141.9,112.3,4.9,1,1205,-142.5,757.3,1,1217.6,195.1,783.8,1, 
1612.7,25,918.6,1,1612.3,23,915.2,1,1321.2,-137,857.4,1,1127.6,-134.7,833.1,1,1146.4,-127.5,444.6,1,993.6,-129.6,454.6,1,1186.2,-125.5,374,1,1112.7,-182.7,254.8,1,1149.7,-111,381,1,1414.5,-82.3,173.2,1,1468.5,-59.6,189.4,1,1399.6,-107.5,76.3,1,1307.5,185.3,886.3,1,1117.3,172.4,859.6,1,1144.3,140.9,430.5,1,1000.9,100.2,457.1,1,1170,144.1,358.8,1,1034.6,165,295,1,1150.1,125,376.5,1,1216.3,92.2,34.7,1,1267.9,79.8,7,1,1141.8,112.2,5,1,1198.9,-141.4,758.4,1,1212.2,193.3,783.9,1, 
1608.2,28.4,920.1,1,1605.8,24.5,915.8,1,1314.7,-137.3,859,1,1121.5,-133.7,834.7,1,1135.5,-125.7,446.4,1,982.1,-129,459.2,1,1173.2,-124.9,375.6,1,1097.8,-182,257.9,1,1137.2,-109.8,383,1,1397.1,-82.8,170.2,1,1451.2,-62.2,184.8,1,1379.5,-108,74.2,1,1301.1,187.1,886.1,1,1111.6,172.2,860.2,1,1142.4,141.1,430.3,1,999.2,98.1,455,1,1168.8,144,358.7,1,1033.6,163.5,294.5,1,1148.7,124.8,376.3,1,1216.3,91.9,34.7,1,1266.6,82.6,7,1,1142.1,111.3,4.6,1,1192.8,-140.4,759.6,1,1205.9,194.2,784.1,1, 
1601.7,27.7,920.5,1,1600.4,22.5,916.4,1,1308.1,-137.6,860.4,1,1115.3,-133.3,836.1,1,1123.9,-125.6,448.6,1,970.7,-129.6,464.4,1,1161,-122.9,377,1,1082.8,-182.2,261.2,1,1125,-108.9,385.1,1,1379,-84.5,167.3,1,1432.6,-67,180.7,1,1358.1,-111,72.7,1,1295.5,186.9,886.6,1,1105.4,173.9,860.3,1,1140.3,141.1,429.9,1,997.1,97,453.1,1,1167.3,144.3,358.5,1,1032.6,162.8,293.9,1,1147.7,123.8,376,1,1215.7,93.3,35,1,1266.4,83.3,7.1,1,1141.9,111.4,4.7,1,1186.6,-140,760.7,1,1199.6,196,784.4,1, 
1596.2,29,921.3,1,1593.5,24.1,917,1,1301.8,-137.2,861.9,1,1109.4,-131.6,837.4,1,1112.4,-126.5,451,1,960.1,-127.7,469.2,1,1148.8,-120.9,378.5,1,1067.7,-182.8,264.7,1,1112.9,-107.4,387.2,1,1360.8,-86,163.9,1,1416.3,-65.1,175,1,1337.8,-110.2,69.6,1,1289.9,187.4,886.4,1,1099.8,173.6,860.3,1,1139.1,137.8,429.3,1,995.9,96.6,451.4,1,1166.4,143.1,358.4,1,1031.2,163.4,293.1,1,1146.4,123.3,375.8,1,1216,92,34.9,1,1267.2,79.9,6.8,1,1142,111.5,4.8,1,1179.4,-141.9,761.8,1,1194.1,194.4,784.5,1, 
1590.2,28.9,921.6,1,1588.6,21.4,917.8,1,1295.4,-137.5,863,1,1102.3,-133,838.9,1,1101.4,-126,453.1,1,948.8,-129.7,474.5,1,1136.9,-119.5,379.8,1,1053.3,-181.3,267.9,1,1100.4,-107.9,389.4,1,1343.2,-85,160.1,1,1398.1,-66.7,170,1,1318.1,-108.1,65.8,1,1283.2,189.8,886.6,1,1094,174.2,860.8,1,1136.6,139.3,429.4,1,993.8,94.1,449.7,1,1165,144.3,358.4,1,1030.7,162.9,292.4,1,1145.5,122.9,375.6,1,1216.1,91.3,34.7,1,1267.2,78.8,6.5,1,1142.1,110.7,4.8,1,1172.7,-141.3,763,1,1188,194.9,784.8,1, 
1584,28.3,922.1,1,1582.4,21.2,918,1,1289,-137.1,864.6,1,1096,-132.5,840.2,1,1090.7,-125.1,455.2,1,938.5,-129.4,479.5,1,1124.5,-118.6,381.1,1,1038.6,-181.6,271.4,1,1088.1,-108.1,391.5,1,1325.5,-84.3,156,1,1379.6,-68.6,164.5,1,1297,-109,62.5,1,1277.5,189.9,886.5,1,1088.5,173.6,860.9,1,1134.4,139.7,429.1,1,991.7,95.6,448.2,1,1164.2,143.4,358.3,1,1030.2,162.6,291.3,1,1144.1,123.1,375.4,1,1216,92,34.8,1,1266.2,81.7,7,1,1141.8,111.7,4.8,1,1166.2,-141.3,764,1,1182.1,194.8,784.7,1, 
	};
}