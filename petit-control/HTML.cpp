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
"     <form action=\"/simple_mode\" method=\"post\">"
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
" </div>";
ret += html_stepped_temperatures();
ret += "</div>"
"</body>"
"</html>";

return ret;
}

String html_stepped_temperatures(){
  // https://forum.arduino.cc/index.php?topic=548582.15
  // https://en.cppreference.com/w/cpp/language/string_literal
  String stepped_temps_html = R"=====(
      <div class="panel panel-default">
        <div class="panel-heading">
          Modo escalonado <span class="hidden-xs">(Temperatura escalonada segun parametros)</span>
        </div>
        <div class="panel-body">
          <form action="/stepped_mode" method="POST">
            <table class="table" width="100%">
              <thead>
                <th>Escalon</th>
                <th>Temperatura</th>
                <th>Tiempo</th>
              </thead>
              <tbody>
                <!-- Step 1 -->
                <tr>
                  <td>1</td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_1_temperature" class="form-control" value="{{step_1_temperature}}" min="0">
                      <span class="input-group-addon">º</span>
                    </div>
                  </td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_1_hours" class="form-control" value="{{step_1_hours}}" min="0">
                      <span class="input-group-addon">Hs</span>
                    </div>
                  </td>
                </tr>

                <!-- Step 2 -->
                <tr>
                  <td>2</td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_2_temperature" class="form-control" value="{{step_2_temperature}}" min="0">
                      <span class="input-group-addon">º</span>
                    </div>
                  </td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_2_hours" class="form-control" value="{{step_2_temperature}}"min="0">
                      <span class="input-group-addon">Hs</span>
                    </div>
                  </td>
                </tr>

                <!-- Step 3 -->
                <tr>
                  <td>3</td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_3_temperature" class="form-control" value="{{step_3_temperature}}" min="0">
                      <span class="input-group-addon">º</span>
                    </div>
                  </td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_3_hours" class="form-control" value="{{step_3_hours}}" min="0">
                      <span class="input-group-addon">Hs</span>
                    </div>
                  </td>
                </tr>

                <!-- Step 4 -->
                <tr>
                  <td>4</td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_4_temperature" class="form-control" value="{{step_4_temperature}}" min="0">
                      <span class="input-group-addon">º</span>
                    </div>
                  </td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_4_hours" class="form-control"value="{{step_4_hours}}" min="0">
                      <span class="input-group-addon">Hs</span>
                    </div>
                  </td>
                </tr>

                <!-- Step 5 -->
                <tr>
                  <td>5</td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_5_temperature" class="form-control" value="{{step_5_temperature}}" min="0">
                      <span class="input-group-addon">º</span>
                    </div>
                  </td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_5_hours" class="form-control" value="{{step_5_hours}}" min="0">
                      <span class="input-group-addon">Hs</span>
                    </div>
                  </td>
                </tr>

                <!-- Step 6 -->
                <tr>
                  <td>6</td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_6_temperature" class="form-control" value="{{step_6_temperature}}" min="0">
                      <span class="input-group-addon">º</span>
                    </div>
                  </td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_6_hours" class="form-control" value="{{step_6_hours}}" min="0">
                      <span class="input-group-addon">Hs</span>
                    </div>
                  </td>
                </tr>

                <!-- Step 7 -->
                <tr>
                  <td>7</td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_7_temperature" class="form-control" value="{{step_7_temperature}}" min="0">
                      <span class="input-group-addon">º</span>
                    </div>
                  </td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_7_hours" class="form-control" value="{{step_7_hours}}" min="0">
                      <span class="input-group-addon">Hs</span>
                    </div>
                  </td>
                </tr>

                <!-- Step 8 -->
                <tr>
                  <td>8</td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_8_temperature" class="form-control" value="{{step_8_temperature}}" min="0">
                      <span class="input-group-addon">º</span>
                    </div>
                  </td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_8_hours" class="form-control" value="{{step_8_hours}}" min="0">
                      <span class="input-group-addon">Hs</span>
                    </div>
                  </td>
                </tr>

                <!-- Step 9 -->
                <tr>
                  <td>9</td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_9_temperature" class="form-control" value="{{step_9_temperature}}" min="0">
                      <span class="input-group-addon">º</span>
                    </div>
                  </td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_9_hours" class="form-control" value="{{step_9_hours}}" min="0">
                      <span class="input-group-addon">Hs</span>
                    </div>
                  </td>
                </tr>

                <!-- Step 10 -->
                <tr>
                  <td>10</td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_10_temperature" class="form-control" value="{{step_10_temperature}}" min="0">
                      <span class="input-group-addon">º</span>
                    </div>
                  </td>
                  <td>
                    <div class="input-group">
                      <input type="number" name="step_10_hours" class="form-control" value="{{step_10_hours}}" min="0">
                      <span class="input-group-addon">Hs</span>
                    </div>
                  </td>
                </tr>
                
              </tbody>
            </table>

            <button type="submit" class="btn btn-primary pull-right"> Activar modo escalonado</button>
          </form>
        </div>
      </div>
    )=====";

  stepped_temps_html.replace("{{step_1_temperature}}", temperatureSteps[0]->temperature > 0 ? (const char*) temperatureSteps[0]->temperature : "0");
  stepped_temps_html.replace("{{step_2_temperature}}", temperatureSteps[1]->temperature > 0 ? (const char*) temperatureSteps[1]->temperature : "0");
  stepped_temps_html.replace("{{step_3_temperature}}", temperatureSteps[2]->temperature > 0 ? (const char*) temperatureSteps[2]->temperature : "0");
  stepped_temps_html.replace("{{step_4_temperature}}", temperatureSteps[3]->temperature > 0 ? (const char*) temperatureSteps[3]->temperature : "0");
  stepped_temps_html.replace("{{step_5_temperature}}", temperatureSteps[4]->temperature > 0 ? (const char*) temperatureSteps[4]->temperature : "0");
  stepped_temps_html.replace("{{step_6_temperature}}", temperatureSteps[5]->temperature > 0 ? (const char*) temperatureSteps[5]->temperature : "0");
  stepped_temps_html.replace("{{step_7_temperature}}", temperatureSteps[6]->temperature > 0 ? (const char*) temperatureSteps[6]->temperature : "0");
  stepped_temps_html.replace("{{step_8_temperature}}", temperatureSteps[7]->temperature > 0 ? (const char*) temperatureSteps[7]->temperature : "0");
  stepped_temps_html.replace("{{step_9_temperature}}", temperatureSteps[8]->temperature > 0 ? (const char*) temperatureSteps[8]->temperature : "0");
  stepped_temps_html.replace("{{step_10_temperature}}", temperatureSteps[9]->temperature > 0 ? (const char*) temperatureSteps[9]->temperature : "0");

  stepped_temps_html.replace("{{step_1_hour}}", temperatureSteps[0]->temperature > 0 ? (const char*) temperatureSteps[0]->hours : "0");
  stepped_temps_html.replace("{{step_2_hour}}", temperatureSteps[1]->temperature > 0 ? (const char*) temperatureSteps[1]->hours : "0");
  stepped_temps_html.replace("{{step_3_hour}}", temperatureSteps[2]->temperature > 0 ? (const char*) temperatureSteps[2]->hours : "0");
  stepped_temps_html.replace("{{step_4_hour}}", temperatureSteps[3]->temperature > 0 ? (const char*) temperatureSteps[3]->hours : "0");
  stepped_temps_html.replace("{{step_5_hour}}", temperatureSteps[4]->temperature > 0 ? (const char*) temperatureSteps[4]->hours : "0");
  stepped_temps_html.replace("{{step_6_hour}}", temperatureSteps[5]->temperature > 0 ? (const char*) temperatureSteps[5]->hours : "0");
  stepped_temps_html.replace("{{step_7_hour}}", temperatureSteps[6]->temperature > 0 ? (const char*) temperatureSteps[6]->hours : "0");
  stepped_temps_html.replace("{{step_8_hour}}", temperatureSteps[7]->temperature > 0 ? (const char*) temperatureSteps[7]->hours : "0");
  stepped_temps_html.replace("{{step_9_hour}}", temperatureSteps[8]->temperature > 0 ? (const char*) temperatureSteps[8]->hours : "0");
  stepped_temps_html.replace("{{step_10_hours}}", temperatureSteps[9]->temperature > 0 ? (const char*) temperatureSteps[9]->hours : "0");

  return stepped_temps_html;
}

void bootstrap()
{
  File file = SPIFFS.open("/bootstrap.min.css.gz", "r"); 
  size_t sent = server.streamFile(file, "text/css");
}
