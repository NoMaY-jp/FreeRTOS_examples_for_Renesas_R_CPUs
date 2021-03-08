// Modify the r_cg_vector_table.c so that user can customize vectors.
// cscript -nologo r_cg_vector_table.js r_cg_vector_table.c r_cg_vector_table.h

var adTypeText = 2; // ADO StreamTypeEnum Value
var adSaveCreateOverWrite = 2; // ADO SaveOptionsEnum Value
var adLF = 10; // ADO LineSeparatorEnum Value
var adCR = 13; // ADO LineSeparatorEnum Value
var adCRLF = -1; // ADO LineSeparatorEnum Value

var fso = new ActiveXObject("Scripting.FileSystemObject");
var srcstrm = new ActiveXObject("ADODB.Stream");
var dststrm = new ActiveXObject("ADODB.Stream");

var filepath  = WScript.Arguments(0);
var c_srcfile = WScript.Arguments(1);
var c_dstfile = WScript.Arguments(1);
var h_srcfile = WScript.Arguments(2);
var h_dstfile = WScript.Arguments(2);

srcstrm.Type = adTypeText;
srcstrm.Charset = "_autodetect_all";
srcstrm.LineSeparator = adCRLF;

dststrm.Type = adTypeText;
dststrm.Charset = "_autodetect_all";
dststrm.LineSeparator = adCRLF;

// c file (r_cg_vector_table.c)

srcstrm.Open();
srcstrm.LoadFromFile(filepath + "\\" + c_srcfile);
text = srcstrm.ReadText();
srcstrm.Close();

text = text.replace("#include \"" + h_dstfile + "\"\r\n", "");
text = text.replace(/(\r\n#include .*\r\n)([^#])/, "$1#include \"" + h_dstfile + "\"\r\n$2");

text = text.replace(/\t\/\/ Address (0x.*)\r\n\tR_Dummy,\r\n/g, "\t// Address $1\r\n\tR_Vector_$1,\r\n");

dststrm.Open();
dststrm.WriteText(text);
dststrm.SaveToFile(filepath + "\\" + c_dstfile, adSaveCreateOverWrite);
dststrm.Close();

// h file (r_cg_vector_table.h)

srcstrm.Open();
srcstrm.LoadFromFile(filepath + "\\" + h_srcfile);
text = srcstrm.ReadText();
srcstrm.Close();

/* Nothing to do. (It is intended to update the time stump of the file.) */

dststrm.Open();
dststrm.WriteText(text);
dststrm.SaveToFile(filepath + "\\" + h_dstfile, adSaveCreateOverWrite);
dststrm.Close();
