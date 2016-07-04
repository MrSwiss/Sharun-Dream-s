
<?php
$lists = array();
for ($i=1; count($_SERVER["argv"]) > $i; $i++) {
	$fp = fopen($_SERVER["argv"][$i], "r");
	if ($fp) {
		while (fscanf($fp, "%s = %i", $n, $zz)) {
			if (!in_array($n, $lists))
				$lists[] = $n;
		}
		fclose($fp);
	}
}

$data = "#ifndef _OPCODE_GEN_HPP_\n";
$data .= "#define _OPCODE_GEN_HPP_\n\n";

$i = 0;
$data .= "enum OpCode_Enum {\n";
foreach ($lists as $row)
	$data .= "\t$row".($i++?"":" = 0").",\n";
$data .= "\tOPC_MAX\n";
$data .= "};\n";
$data .= "\n";
$data .= "#endif // _OPCODE_GEN_HPP_\n";
file_put_contents("OpCode_Gen.hpp", $data);

$data = "#include \"Sharun.hpp\"\n\n";
$data .= "const char *OpCode_Gen[] = {\n";
foreach ($lists as $row)
	$data .= "\t\"$row\",\n";
$data .= "\tNULL\n";
$data .= "};\n";
file_put_contents("OpCode_Gen.cpp", $data);

?>
