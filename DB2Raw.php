<?php
  $user="root";
  $pass="pass";
  $server = "127.0.0.1";
  $bd = "Sharun_Dream";
  $db_link = mysqli_connect($server, $user,$pass) or die ("errer SGBD");
  mysqli_select_db($db_link, $bd) or die ("errer Table");

/*
c	signed char
C 	unsigned char
s 	signed short (always 16 bit, machine byte order)
S 	unsigned short (always 16 bit, machine byte order)
i 	signed integer (machine dependent size and byte order)
I 	unsigned integer (machine dependent size and byte order)
l 	signed long (always 32 bit, machine byte order)
L 	unsigned long (always 32 bit, machine byte order)
f 	float (machine dependent size and representation)
d 	double (machine dependent size and representation)
*/

/*
	uint	Skill_Id;
	ushort	Mount_Id;
	ushort	Speed;
	uchar	R_Stamina;
	uchar	R_HP;
	uchar	R_MP;
*/
  echo "-> mounts.db...";
  $fp = fopen("Sharun_Saves/DB/mounts.db", "wb");
  $result1 = mysqli_query($db_link, "SELECT * FROM `mounts`;") or die(mysql_error());
  while ($row = mysqli_fetch_row($result1)) {
    fwrite($fp, pack("I", $row[0]), 4);
    fwrite($fp, pack("S", $row[1]), 2);
    fwrite($fp, pack("S", $row[2]), 2);
    fwrite($fp, pack("C", $row[3]), 1);
    fwrite($fp, pack("C", $row[4]), 1);
    fwrite($fp, pack("C", $row[5]), 1);
  }
  fclose($fp);
  echo " Ok!\n";

/*
	uint	id;
	char	category[30];
	uchar	level;
	char	type[30];
	short	cooldown;
	short	rank;
	char	rareGrade[10];
	uchar	slotLimit;
	short	maxStack;
	uchar	ability;
	uint	buyPrice;
	uint	sellPrice;
	uint	skill_id;
	uchar	c_slots;
	ushort	atk;
	ushort	def;
	ushort	bal;
	ushort	ipt;
	uchar	bind;
*/
  echo "-> common_items.db...";
  $fp = fopen("Sharun_Saves/DB/common_items.db", "wb");
  $result1 = mysqli_query($db_link, "SELECT * FROM `common_items` ORDER BY `type`, `category`;") or die(mysql_error());
  while ($row = mysqli_fetch_row($result1)) {
    fwrite($fp, pack("I", $row[ 0]), 4);
    fwrite($fp, $row[ 1], 30);
    for ($i = strlen($row[ 1]); $i<30; $i++)
      fwrite($fp, pack("C", 0), 1);
    fwrite($fp, pack("C", $row[ 3]), 1);
    fwrite($fp, $row[ 4], 30);
    for ($i = strlen($row[ 4]); $i<30; $i++)
      fwrite($fp, pack("C", 0), 1);
    fwrite($fp, pack("S", $row[ 5]), 2);
    fwrite($fp, pack("S", $row[ 6]), 2);
    fwrite($fp, $row[ 7], 10);
    for ($i = strlen($row[ 7]); $i<10; $i++)
      fwrite($fp, pack("C", 0), 1);
    fwrite($fp, pack("C", $row[ 8]), 1);
    fwrite($fp, pack("S", $row[ 9]), 2);
    $ability = 0;
    if ($row[10]) $ability |= 1; // ITEM_CAN_TRADE
    if ($row[11]) $ability |= 2; // ITEM_CAN_DESTROY
    if ($row[12]) $ability |= 4; // ITEM_CAN_BANK
    if ($row[13]) $ability |= 8; // ITEM_CAN_GUILD
    fwrite($fp, pack("C", $ability), 1);
    fwrite($fp, pack("I", $row[14]), 4);
    fwrite($fp, pack("I", $row[15]), 4);
    fwrite($fp, pack("I", $row[16]), 4);
    fwrite($fp, pack("C", 0), 1);
    fwrite($fp, pack("S", 0), 2);
    fwrite($fp, pack("S", 0), 2);
    fwrite($fp, pack("S", 0), 2);
    fwrite($fp, pack("S", 0), 2);
    fwrite($fp, pack("C", $row[17]), 1);
  }
  fclose($fp);
  echo " Ok!\n";

  echo "-> equipment_items.db...";
  $fp = fopen("Sharun_Saves/DB/equipment_items.db", "wb");
  $result1 = mysqli_query($db_link, "SELECT * FROM `equipment_items` ORDER BY `type`, `category`;") or die(mysql_error());
  while ($row = mysqli_fetch_row($result1)) {
    fwrite($fp, pack("I", $row[ 0]), 4);
    fwrite($fp, $row[ 1], 30);
    for ($i = strlen($row[ 1]); $i<30; $i++)
      fwrite($fp, pack("C", 0), 1);
    fwrite($fp, pack("C", $row[ 2]), 1);
    fwrite($fp, $row[ 3], 30);
    for ($i = strlen($row[ 3]); $i<30; $i++)
      fwrite($fp, pack("C", 0), 1);
    fwrite($fp, pack("S", $row[ 4]), 2);
    fwrite($fp, pack("S", $row[ 5]), 2);
    fwrite($fp, $row[ 6], 10);
    for ($i = strlen($row[ 6]); $i<10; $i++)
      fwrite($fp, pack("C", 0), 1);
    fwrite($fp, pack("C", $row[ 7]), 1);
    fwrite($fp, pack("S", $row[ 8]), 2);
    $ability = 0;
    if ($row[14]) $ability |= 1; // ITEM_CAN_TRADE
    if ($row[15]) $ability |= 2; // ITEM_CAN_DESTROY
    if ($row[16]) $ability |= 4; // ITEM_CAN_BANK
    if ($row[17]) $ability |= 8; // ITEM_CAN_GUILD
    fwrite($fp, pack("C", $ability), 1);
    fwrite($fp, pack("I", $row[18]), 4);
    fwrite($fp, pack("I", $row[19]), 4);
    fwrite($fp, pack("I", $row[16]), 4);
    fwrite($fp, pack("C", $row[ 9]), 1);
    fwrite($fp, pack("S", $row[10]), 2);
    fwrite($fp, pack("S", $row[11]), 2);
    fwrite($fp, pack("S", $row[12]), 2);
    fwrite($fp, pack("S", $row[13]), 2);
    fwrite($fp, pack("C", $row[20]), 1);
  }
  fclose($fp);
  echo " Ok!\n";

/*
	uint	Id;
	void*	Friendly;
	void*	Monsters;
	void*	next;
*/
  $i = 0;
  $ready = [];
  echo "-> zones.db...";
  $fp = fopen("Sharun_Saves/DB/zones.db", "wb");
  $result1 = mysqli_query($db_link, "SELECT `Zones` FROM `Zones_HuntingZone` WHERE `Zones` NOT IN ( SELECT `from` FROM `Zones_Channeling`);") or die(mysql_error());
  while ($row = mysqli_fetch_row($result1)) {
    if (!in_array($row[ 0], $ready)) {
      fwrite($fp, pack("I", $row[ 0]), 4);
      $ready[] = $row[ 0];
    }
  }
  fclose($fp);
  echo " Ok!\n";

/*
	uchar	strcut_type;
	char	spawned; // >1=spawned , 0=dead
	uint	Id;
	uint	SUBID;
	uint	npc_id;
	uint	npc_type;
	char	name[141];
	int	huntingZone;
	ushort	level;
	uint	hpBase;
	uint	attack;
	uint	impact;
	uint	defense;
	uint	balance;
	CHAR_LOC_t	World;
*/
    foreach ($ready as $zone) {
      for ($i=0; $i<2; $i++) {
	      echo "-> ".($i?"mob":"npc")."_$zone.db...";
	      $fp = fopen(($i?"mob":"npc")."_$zone.db", "wb");
	      $result1 = mysqli_query($db_link, "SELECT * FROM `npcs`, `npc_locs` WHERE `class`".($i?"<>":"=")."'FRIENDLY' AND `id`=`npc_id` AND `map_id`='$zone' AND `huntingZone`=`npc_type` AND `huntingZone` IN (SELECT `HuntingZone` FROM `Zones_HuntingZone` , `Zones_Channeling` WHERE (`Zones` = `from` AND `to` = '$zone') OR `Zones` = '$zone' GROUP BY `HuntingZone`);") or die(mysql_error());
	      while ($row = mysqli_fetch_row($result1)) {
	//	uint	npc_id;
		fwrite($fp, pack("I", $row[ 0]), 4);
	//	uint	npc_type;
		fwrite($fp, pack("I", $row[ 2]), 4);
	//	char	name[141];
		fwrite($fp, $row[ 1], 141);
		for ($i = strlen($row[ 1]); $i<141; $i++)
		  fwrite($fp, pack("C", 0), 1);
	//	int	huntingZone;
		fwrite($fp, pack("i", $row[ 2]), 4);
	//	ushort	level;
		fwrite($fp, pack("S", $row[ 4]), 2);
	//	uint	hpBase;
		fwrite($fp, pack("I", $row[ 9]), 4);
	//	uint	attack;
		fwrite($fp, pack("I", $row[ 5]), 4);
	//	uint	impact;
		fwrite($fp, pack("I", $row[ 6]), 4);
	//	uint	defense;
		fwrite($fp, pack("I", $row[ 7]), 4);
	//	uint	balance;
		fwrite($fp, pack("I", $row[ 8]), 4);
	//	CHAR_LOC_t	World;
		// float	XYZ[3]
		fwrite($fp, pack("f", $row[16]), 4);
		fwrite($fp, pack("f", $row[17]), 4);
		fwrite($fp, pack("f", $row[18]), 4);
		fwrite($fp, pack("S", $row[19]), 2);
	      }
	      fclose($fp);
	      echo " Ok!\n";
      }
    }
?>
