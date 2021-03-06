String sendHTML(float temperature, int rssi){
  String pg = "";
  pg += "<!DOCTYPE html>\n";
  pg += "<html>\n";
  pg += "<head>\n";
  pg += "<title>" + String(TITLE) + "</title>\n";
  pg += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
  pg += "<link href='https://fonts.googleapis.com/css?family=Open+Sans:300,400,600' rel='stylesheet'>\n";
  pg += "<style>\n";
  pg += "html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #444444;}\n";
  pg += "body{margin: 0px;} \n";
  pg += "h1 {margin: 50px auto 0px;} \n";
  pg += "h2 {margin: 0px auto 10px;} \n";
  pg += "h3 {margin: 0px auto 50px;} \n";
  pg += ".side-by-side{display: table-cell;vertical-align: middle;position: relative;}\n";
  pg += ".text{font-weight: 600;font-size: 19px;width: 200px;}\n";
  pg += ".reading{font-weight: 300;font-size: 50px;padding-right: 25px;}\n";
  pg += ".temperature .reading{color: #26B999;}\n";
  pg += ".temperatureSet .reading{color: #F29C21;}\n";
  pg += ".humidity .reading{color: #3B97D3;}\n";
  pg += ".phasestatus .reading{color: #955BA5;}\n";
  pg += ".altitude .reading{color: #f24e21;}\n";
  pg += ".superscript{font-size: 17px;font-weight: 600;position: absolute;top: 10px;}\n";
  pg += ".data{padding: 10px;}\n";
  pg += ".container{display: table;margin: 0 auto;}\n";
  pg += ".icon{width:65px}\n";
  pg += "</style>\n";
  pg += "</head>\n";
  pg += "<body>\n";
  pg += "<h1>" + String(SOFTWARE_NAME) + "</h1>\n";
  pg += "<h2>" + String(MODULE_NAME) + "</h2>\n";
  pg += "<h3>" + String(VER) + "</h3>\n";
  pg += "<div class='container'>\n";
  
  
  pg += "<div class='data temperature'>\n";
  pg += "<div class='side-by-side icon'>\n";
  pg += "<svg enable-background='new 0 0 19.438 54.003'height=54.003px id=Layer_1 version=1.1 viewBox='0 0 19.438 54.003'width=19.438px x=0px xml:space=preserve xmlns=http://www.w3.org/2000/svg xmlns:xlink=http://www.w3.org/1999/xlink y=0px><g><path d='M11.976,8.82v-2h4.084V6.063C16.06,2.715,13.345,0,9.996,0H9.313C5.965,0,3.252,2.715,3.252,6.063v30.982\n";
  pg += "C1.261,38.825,0,41.403,0,44.286c0,5.367,4.351,9.718,9.719,9.718c5.368,0,9.719-4.351,9.719-9.718\n";
  pg += "c0-2.943-1.312-5.574-3.378-7.355V18.436h-3.914v-2h3.914v-2.808h-4.084v-2h4.084V8.82H11.976z M15.302,44.833\n";
  pg += "c0,3.083-2.5,5.583-5.583,5.583s-5.583-2.5-5.583-5.583c0-2.279,1.368-4.236,3.326-5.104V24.257C7.462,23.01,8.472,22,9.719,22\n";
  pg += "s2.257,1.01,2.257,2.257V39.73C13.934,40.597,15.302,42.554,15.302,44.833z'fill=#26B999 /></g></svg>\n";
  pg += "</div>\n";
  pg += "<div class='side-by-side text'>Temperature</div>\n";
  pg += "<div class='side-by-side reading'>\n";
  pg += "<b>" + String(temperature) + "</b>\n";
  pg += "<span class='superscript'>&deg;C</span></div>\n";
  pg += "</div>\n";

  pg += "<div class='data tech'>\n";
  pg += "<div class='side-by-side icon'>\n";
  pg += "<svg enable-background='new 0 0 40.542 40.541'height=40.541px id=Layer_1 version=1.1 viewBox='0 0 40.542 40.541'width=40.542px x=0px xml:space=preserve xmlns=http://www.w3.org/2000/svg xmlns:xlink=http://www.w3.org/1999/xlink y=0px><g><path d='M34.313,20.271c0-0.552,0.447-1,1-1h5.178c-0.236-4.841-2.163-9.228-5.214-12.593l-3.425,3.424\n";
  pg += "c-0.195,0.195-0.451,0.293-0.707,0.293s-0.512-0.098-0.707-0.293c-0.391-0.391-0.391-1.023,0-1.414l3.425-3.424\n";
  pg += "c-3.375-3.059-7.776-4.987-12.634-5.215c0.015,0.067,0.041,0.13,0.041,0.202v4.687c0,0.552-0.447,1-1,1s-1-0.448-1-1V0.25\n";
  pg += "c0-0.071,0.026-0.134,0.041-0.202C14.39,0.279,9.936,2.256,6.544,5.385l3.576,3.577c0.391,0.391,0.391,1.024,0,1.414\n";
  pg += "c-0.195,0.195-0.451,0.293-0.707,0.293s-0.512-0.098-0.707-0.293L5.142,6.812c-2.98,3.348-4.858,7.682-5.092,12.459h4.804\n";
  pg += "c0.552,0,1,0.448,1,1s-0.448,1-1,1H0.05c0.525,10.728,9.362,19.271,20.22,19.271c10.857,0,19.696-8.543,20.22-19.271h-5.178\n";
  pg += "C34.76,21.271,34.313,20.823,34.313,20.271z M23.084,22.037c-0.559,1.561-2.274,2.372-3.833,1.814\n";
  pg += "c-1.561-0.557-2.373-2.272-1.815-3.833c0.372-1.041,1.263-1.737,2.277-1.928L25.2,7.202L22.497,19.05\n";
  pg += "C23.196,19.843,23.464,20.973,23.084,22.037z'fill=#8a8a8a /></g></svg>\n";
  pg += "</div>\n";
  pg += "<div class='side-by-side text'>RSSI</div>\n";
  pg += "<div class='side-by-side reading'>\n";
  pg += String(rssi) + "\n";
  pg += "</div>\n";
  
  pg += "</div>\n";
  pg += "</div>\n";
  pg += "</body></html>\n";
  
  return pg;
}
