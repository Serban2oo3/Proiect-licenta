        const char PAGE_MAIN[] PROGMEM = R"=====(
        <!DOCTYPE html>
        <html>

        <head>
            <meta charset ="utf-8">
            <meta name ="viewport" content="width=device-width, initial-scale=1">
            <title>Smart.HM</title>
        </head>

        <style>
            body{
                background-color : #3eebff;
                margin : 0;
                color:black;
                display: flex;
                flex-direction :column;
                align-items: center;
            }
            header{
                width:35%;
                padding : 20px 0;
                background-color: white;
                text-align:  center;
                border-radius: 20px;
                box-shadow: 0 5px 11px rgba(0,0,0,0.3);
            }
            header h1{
                font-size: 24px;
                margin: 15px;
                color: black;;
                letter-spacing: 2px;
            }
            .iluminare{
                display :flex;
                justify-content: center;
                gap: 50px;
                margin-top: 30px;
            }
            .box{
                width :200px;
                height: 200px;
                background-color: white ;
                color :black;
                border-radius: 20px;
                box-shadow: 0 5px 11px rgba(0,0,0,0.3);
                padding : 15 px;
                text-align : center;
                position: relative;
            }
            .box p
            {
                font-size: 20px;
                
                margin-bottom:15px;
            }
        .buton_on_off
            {
                position: relative;
                display: inline-block;
                width: 70px;
                height: 36px;
            }

            .buton_on_off input
            {
                opacity: 0.1;
                width: 0.1;
                height: 0.1;
            }

            .glisare   
            {
                position: absolute;
                    top: 0;
                left: 0;
                right: 0;
                bottom: 0;
                background: linear-gradient(145deg, #d1d1d1, #f0f0f0);
                border-radius: 40px;
                transition: background-color 0.3s ease-in-out;
                box-shadow: inset 0 0 5px rgba(0,0,0,0.2);
            }

            .glisare::before 
            {
                content: "";
                position: absolute;
                height: 28px;
                width: 28px;
                left: 4px;
                top: 4px;
                background-color: #ffffff;
                border-radius: 50%;
                transition: transform 0.3s ease-in-out, background-color 0.3s;
                box-shadow: 0 2px 5px rgba(0, 0, 0, 0.2);
            }           

            input:checked + .glisare
            {
                background: linear-gradient(145deg, #4facfe, #00f2fe);
                box-shadow: 0 0 5px #4facfe, 0 0 15px #00f2fe;
            }

            input:checked + .glisare::before
            {
                transform: translateX(34px);
                background-color: #e0f7ff;
            }

            input:checked + .glisare
            {
                background-color: #2196F3;
            }
            input:checked + .glisare:before
            {
                transform: translateX(28px);
            }
            .section-header {
                font-size :24px;
                width:35%;
                padding : 20px 0;
                font-size: 24px;
                margin-top: 20px;
                margin-bottom: 20px;
                background-color: white;
                text-align:  center;
                border-radius: 20px;
                box-shadow: 0 5px 11px rgba(0,0,0,0.3);
            }
            table{
                width: 50%;
                border-collapse: collapse;
                margin-bottom : 40px;
                background-color: rgb(193, 193, 193);
                color :rgb(17, 12, 12);
                
                overflow: hidden;
                border-radius: 27px;
                box-shadow: 1 5px 11px rgba(7, 1, 1, 0.3);
            }
            th, td {
            padding: 15px;
            text-align: center;
            font-size: 18px;
            }

            th {
            background-color: #3c465a;
            color: white;
            }

            tr:nth-child(even) {
            background-color: #f2f2f2;
            }

            tr:hover {
            background-color: #ddd;
            }
            .temp-control {
            display: flex;
            justify-content: center;
            gap: 50px;
            margin-bottom: 40px;
            margin-top: 30px;
        }



        </style>
        

        <body>
            <header><h1>Light monitor</h1></header>


            <div class="iluminare">
                <div class="box">
                    <p>Lumina sufragerie</p>
                    <label class="buton_on_off">
                        <input type ="checkbox" id="led1" onchange="Switch_Lumina_1 ()">
                        <span class="glisare"></span>
                    </label>

                </div>

                <div class="box">
                    <p>Lumina garaj</p>
                    <label class="buton_on_off">
                        <input type ="checkbox" id="led2" onchange="Switch_Lumina_2 ()">
                        <span class="glisare"></span>
                    </label>

                </div>

                <div class="box">
                    <p>Lumina din fata</p>
                    <p id="led3status">Status: Necunoscut</p>
                </div>
            </div>

            <div class="section-header">Sensor Monitor</div>

            <table>
                <thead>
                    <tr>
                        <th><div class="heading">Sensor</div></th>
                        <th><div class="heading">Valoare</div></th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td><div class="titlutabel">Luminozitate</div></td>
                        <td><div class="valorilesenzoriilor" id="lux"></div></td>
                    </tr>
                    <tr>
                        <td><div class="titlutabel">Temperatură</div></td>
                        <td><div class="valorilesenzoriilor" id="temp"></div></td>
                    </tr>
                    <tr>
                        <td><div class="titlutabel">Presiune</div></td>
                        <td><div class="valorilesenzoriilor" id="presiune"></div></td>
                    </tr>
                    <tr>
                        <td><div class="titlutabel">Altitudine</div></td>
                        <td><div class="valorilesenzoriilor" id="alt"></div></td>
                    </tr>
                </tbody>
            </table>


            <div class="temp-control">
            <div class="box">
                <p>Răcire</p>
                <p style="font-size: 14px;">Va scădea temperatura cu aproximativ 5°C</p>
                <label class="buton_on_off">
                    <input type="checkbox" id="ac" onchange="Racire()">
                    <span class="glisare"></span>
                </label>
            </div>

            <div class="box">
                <p>Încălzire</p>
                <p style="font-size: 14px;">Va crește temperatura cu aproximativ 5°C</p>
                <label class="buton_on_off">
                    <input type="checkbox" id="ca" onchange="Incalzire()">
                    <span class="glisare"></span>
                </label>
            </div>
        </div>

            </div>

        </body>

        <script type ="text/javascript">
            var xmlHttp = XMLObject();

            function XMLObject()
            {
                if (window.XMLHttpRequest){
                    xmlHttp= new XMLHttpRequest();
                }
                else{
                    xmlHttp= new ActiveXObject("Microsoft.XMLHTTP")
                }
                return xmlHttp;
            }

            function Switch_Lumina_1()
            {
                var XL= new XMLHttpRequest();
                var message;
                XL.onreadystatechange=function()
                {
                if(this.readyState==4 && this.status==200)
                message=this.responseText;
                }
                XL.open("PUT", "LUMINA_1", false);
                XL.send();
            }

            function Switch_Lumina_2()
            {
                var XL= new XMLHttpRequest();
                var message;
                XL.onreadystatechange=function()
                {
                if(this.readyState==4 && this.status==200)
                message=this.responseText;
                }
                XL.open("PUT", "LUMINA_2", false);
                XL.send();
            }

            function Incalzire()
            {
                var XL= new XMLHttpRequest();
                var message;
                XL.onreadystatechange=function()
                {
                if(this.readyState==4 && this.status==200)
                message=this.responseText;
                }
                XL.open("PUT", "CA", false);
                XL.send();
            }

            function Racire()
            {
                var XL= new XMLHttpRequest();
                var message;
                XL.onreadystatechange=function()
                {
                if(this.readyState==4 && this.status==200)
                message=this.responseText;
                }
                XL.open("PUT", "AC", false);
                XL.send();
            }

            

            function response()
            {
                if(xmlHttp.readyState==4 && xmlHttp.status==200)
                {
                    try
                    {
                        //sistem iluminare
                        var xmlResponse=xmlHttp.responseXML;
                        if(xmlResponse &&xmlResponse.getElementsByTagName("led1").length>0)
                        {
                            var led1Status = xmlResponse.getElementsByTagName("LED")[0].firstChild.nodeValue;

                        }

                        if(xmlResponse &&xmlResponse.getElementsByTagName("led2").length>0)
                        {
                            var led2Status = xmlResponse.getElementsByTagName("LED2")[0].firstChild.nodeValue;

                        }

                        //led afara
                        var led3 = xmlResponse.getElementsByTagName("LED3")[0].childNodes[0].nodeValue;
                        var statusText = (led3 === "1") ? "Lumina este aprinsă" : "Lumin este stinsă";
                        document.getElementById("led3status").innerHTML = statusText;


                        //aici este tabelu +senzorii

                        if(xmlResponse.getElementsByTagName("LUX")[0])
                        document.getElementById("lux").textContent =xmlResponse.getElementsByTagName("LUX")[0].textContent;

                        if(xmlResponse.getElementsByTagName("TEMPERATURE")[0])
                        document.getElementById("temp").textContent = xmlResponse.getElementsByTagName("TEMPERATURE")[0].textContent;

                        if(xmlResponse.getElementsByTagName("PRESSURE")[0])
                        document.getElementById("presiune").textContent = xmlResponse.getElementsByTagName("PRESSURE")[0].textContent;

                        if(xmlResponse.getElementsByTagName("ALTITUDE")[0])
                        document.getElementById("alt").textContent = xmlResponse.getElementsByTagName("ALTITUDE")[0].textContent;



                        //siste de racire/incalzire
                        if(xmlResponse &&xmlResponse.getElementsByTagName("ca").length>0)
                        {
                            var led3Status = xmlResponse.getElementsByTagName("CA")[0].firstChild.nodeValue;

                        }

                        if(xmlResponse &&xmlResponse.getElementsByTagName("ac").length>0)
                        {
                            var led4Status = xmlResponse.getElementsByTagName("AC")[0].firstChild.nodeValue;

                        }




                    } catch(e){console.error("Eroare la parsarea XML:", e);}

                }
            }

            function process()
            {
                if(xmlHttp.readyState==0 || xmlHttp.readyState==4)
                {
                    xmlHttp.open("PUT", "xml" , true);
                    xmlHttp.onreadystatechange=response;
                    xmlHttp.send(null);
                }
                setTimeout("process()", 1000);

            }
            window.onload = process;

        </script>



        </html>


        )=====";