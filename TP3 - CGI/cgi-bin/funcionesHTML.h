void imprimirHEADER(char seccion[20])
{
	printf ("Content-Type: text/html\n\n");
	printf(
		"<!DOCTYPE html>"
		"<html>"
		"<head>"
			"<title>%s</title>"
			"<LINK REL='Stylesheet' HREF='../estilo.css' TYPE='text/css'>"
		"</head>"
		"<body>"
		
			"<div id='encabezado'>"
								"Sistemas Operativos II - TP3 - WebServer<br>"
								"<span style='font-size:16pt;'>%s</span>"
			"</div>"
			"<div id='bajosombra'></div>"
			"<div id='contenido'>"
		,seccion,seccion);
}
void imprimirFOOTER()
{
	printf(
			"</div>"

		"</body>"
		"</html>"
	);
}
