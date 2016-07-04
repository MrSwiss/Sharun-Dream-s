<?php
$data = "";
$lists = array();
for ($i=1; count($_SERVER["argv"]) > $i; $i++) {
	$fp = fopen($_SERVER["argv"][$i], "r");
	if ($fp) {
		while (!feof($fp)) {
			$n = "";
			$ret = fscanf($fp, " %[^\n]s", $n);
			if ($ret && strpos($n, "extern") !== FALSE && strpos($n, "(*") !== FALSE )//&& !in_array($n, $lists))
				$lists[] = $n;
		}
		fclose($fp);
	}
}

function Clean_Param($row)
{
	$b = 0;
	$e = strrpos($row, "(");
	do {
		$b = $e;
		if ($b !== FALSE && $b < strlen($row)) {
			$e = strpos($row, ",", $b);
			if ($e == FALSE)
				$e = strpos($row, ")", $b);
			if ($e !== FALSE && $b+3 < $e) {
				$b1 = $bx = $b;
				while ($bx !== FALSE && $bx < $e) {
					$b1 = $bx;
					$bx = strpos($row, "*", $b1+1);
				}
				$b2 = $bx = $b;
				while ($bx !== FALSE && $bx < $e) {
					$b2 = $bx;
					$bx = strpos($row, " ", $b2+1);
				}
				if ($b1 == FALSE)
					$b1 = $b2;
				if ($b2 == FALSE)
					$b2 = $b1;
				if ($b1 > $b2)
					$b = $b1;
				else
					$b = $b2;
				if ($b !== FALSE) {
					$b++;
					$row = substr($row, 0, $b) . substr($row, $e);
				} else {
					$b = strrpos($row, " ", $m);
					if ($b !== FALSE)
					$row = substr($row, 0, $b) . substr($row, $e);
				}
//echo "\r-> $row                                 ";
			} else
				$b = FALSE;
		} else
				$b = FALSE;
	} while ($b !== FALSE);
//echo "\n";
	$row = str_replace(" )", ")", $row);
	$row = str_replace(" ,", ",", $row);
	$row = str_replace(" *", "*", $row);
	$row = str_replace("* ", "*", $row);
	return $row;
}

echo "Add Header...\n";
$data .= "#include \"Sharun.hpp\"\n";
$data .= "\n";
$data .= "#ifdef WIN32\n";
$data .= "\n";
$data .= "sharun_t *Sharun;\n";
$data .= "\n";
$data .= "\n";

echo "Add Defines...\n";
foreach ($lists as $row) {
	$row = str_replace("extern ", "", $row);
	$row = str_replace("\t", " ", $row);
	$row = str_replace("  ", " ", $row);
	$row = Clean_Param($row);
	$data .= "$row\n";
}

echo "Add Function...\n";
$data .= "\n";
$data .= "\n";
$data .= "extern \"C\" {\n";
$data .= "\n";
$data .= "void CallBack_Plugin(void** Link)\n";
$data .= "{\n";
$data .= "\tuint i = 0;\n";
$data .= "\tSharun = (sharun_t*) Link[i++];\n";

echo "Add linkings...\n";
foreach ($lists as $row) {
	$row = str_replace(["extern ", ";"], "", $row);
	$row = str_replace("\t", " ", $row);
	$row = str_replace("  ", " ", $row);
	$b = strpos($row, "(*") + 2;
	$e = strpos($row, ")", $b) - $b;
	$var = substr($row, $b, $e);
	$row = str_replace($var, "", $row);
	$row = Clean_Param($row);
	$data .= "\t$var = ($row) Link[i++];\n";
}

echo "Add Foot...\n";
$data .= "}\n";
$data .= "\n";
$data .= "}\n";
$data .= "\n";
$data .= "#endif\n";

echo "Writing...\n";
file_put_contents("CallBack_Plugin.cpp", $data);

echo "Bye ~:)\n";
?>
