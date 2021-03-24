#include "HTML.h"

String html_login(){
    return "<!DOCTYPE HTML>"
    "<html>"
    "<head>"
    "<title>Mason</title>"
    "</head>"
    "<body>"
    "  <FORM action=\"/\" method=\"post\">"
    "    <P>"
    "      Login: "
    "      <INPUT type=\"text\" name=\"password\">  "    
    "      <INPUT type=\"submit\" value=\"Enviar\">"
    "      <br/>"
    "    </P>"
    "  </FORM>"  
    "</body>"
    "</html>";
}

String html_principal(){
String estado="<span class=\"label label-danger\">Off</span>";

if (relay1){
  estado="<span class=\"label label-success\">On</span>";
}
  
String ret="<html><head>"
"    <title>Control Masonico</title>"
" <meta charset=\"utf-8\"> "
" <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
" <link rel=\"stylesheet\" href=\"bootstrap.min.css\">"
"</head>"
""
"<body>"
"<div class=\"container-fluid\">"
" <div class=\"well\">"
"    <h1>Control Masonico</h1>"
" </div>"
" <div class=\"row\">"
"   <div class=\"col-md-6 \">"
"     <form action=\"/ferm1\" method=\"post\">"
"       <div class=\"form-group\">"
"           <label for=\"sarasa\">Temp heladera: <span class=\"label label-primary\"> ";
ret += tempsensada1;
ret+= "°</span></label>"
"           <span class=\"label label-default\">Set: ";
ret += tempset1;
ret+= "°</span>";
ret+= estado;
ret+="           <input type=\"number\" class=\"form-control\" name=\"tempset\" autocomplete=\"off\">"
"       </div>"
"       <div class=\"form-group\">"
"         <input type=\"submit\" class=\"btn btn-default\" value=\"Enviar\"> "
"       </div>"
"     </form>"
"   </div>"
" </div>"
"</div>"
"</body>"
"</html>";
return ret;
}

void bootstrap()
{
  File file = SPIFFS.open("/bootstrap.min.css.gz", "r"); 
  size_t sent = server.streamFile(file, "text/css");
}
