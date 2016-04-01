#!/usr/bin/perl

`echo "#include <stdio.h>\n\n#include \\"OpCodes.h\\"\n#include \\"OpCodes_def_recv.h\\"\n#include \\"OpCodes_def_send.h\\"\n\nconst char *OpCodes_Name[] = {" > OpCodes_Gen.c`;
$i = 0;
foreach $Elem (`cat OpCodes.txt`)
{
  $Elem =~ s/\n//g;
  $Elem =~ s/\r//g;
  `echo "\t\\"$Elem\\"," >> OpCodes_Gen.c`;
  $i++;
}
`echo "\tNULL\n};" >> OpCodes_Gen.c`;

`echo "" > OpCodes_def_recv.h`;
`echo "" > OpCodes_def_send.h`;
`echo "#define OpCodes_Count $i\n#include \\"Player.h\\"\n\nenum OpCodes_Type {\n\tS_LOGIN_FAILED = 0," > OpCodes_Gen.h`;
`echo "\nOpCode_recv_t OpCode_LstR[] = {" >> OpCodes_Gen.c`;
$i = 0;
foreach $Elem (`cat OpCodes.txt`)
{
  $Elem =~ s/\n//g;
  $Elem =~ s/\r//g;
  `echo "#ifdef $Elem\_d_s\n\t$Elem,\n#endif" >> OpCodes_Gen.h`;
  `echo "//#define $Elem\_d_r" >> OpCodes_def_recv.h`;
  `echo "//#define $Elem\_d_s" >> OpCodes_def_send.h`;
  `echo "#ifdef $Elem\_d_r\n\t{NULL, (void*)$Elem\_f, $i},\n#endif" >> OpCodes_Gen.c`;
  $i++;
}
`echo "};" >> OpCodes_Gen.h`;
`echo "\t{\\"done\\"}\n};" >> OpCodes_Gen.c`;

`echo "" >> OpCodes_Gen.h`;
`echo "\nOpCode_send_t OpCode_LstS[] = {" >> OpCodes_Gen.c`;
$i = 0;
foreach $Elem (`cat OpCodes.txt`)
{
  $Elem =~ s/\n//g;
  $Elem =~ s/\r//g;
  `echo "void $Elem\_f(player_t*);" >> OpCodes_Gen.h`;
  `echo "#ifdef $Elem\_d_s\n\t{NULL, (void*)$Elem\_f, $i},\n#endif" >> OpCodes_Gen.c`;
  $init = "";
  $i++;
}
`echo "\t{\\"done\\"}\n};" >> OpCodes_Gen.c`;
