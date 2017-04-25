#ifndef _PAGES_h
#define _PAGES_h

const char index1_html[] PROGMEM = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8" />
    <link rel="stylesheet" type="text/css" href="esp.css">
    <title></title>
</head>
<body>

    <div class="border">
        <div class="title">
            Ustawienia
        </div>
        <div class="content">

            <form action="/config.php" id="data">
                <p>
                    <div class="key">Networks:</div>
                    <select class="val" name="ssid" form="data">
)";

const char index2_html[] PROGMEM = R"(</select>
                </p>
                <p>
                    <div class="key">Password:</div>
                    <input class="val" type="password" name="password">
                </p>

                <p>
                    <div class="key">mqtt_user:</div>
                    <input class="val" type="text" name="mqtt_user" value="pi">
                </p>
                <p>
                    <div class="key">mqtt_pass:</div>
                    <input class="val" type="password" name="mqtt_pass" value="NodeRed">
                </p>
                <p>
                    <div class="key">mqtt_host:</div>
                    <input class="val" type="text" name="mqtt_host" value="pizero">
                </p>
                <p>
                    <div class="key">ota_host:</div>
                    <input class="val" type="text" name="ota_host" value="pizero">
                </p>
                <p>
                    <div class="key">ota_page:</div>
                    <input class="val" type="text" name="ota_page" value="ota.php?dev=">
                </p>
                <p>
                    <input class="sub" type="submit">
                </p>
            </form>

        </div>
        <div class="footer">
            by wito
        </div>
    </div>

</body>
</html>
)";

const char esp_css[] PROGMEM = R"(
body {
    background-color: black;
    font-family: "Consolas";
    font-size: 20px;
}
.border {
    border: 1px solid white;
    margin: 0 auto;
    width: 600px;
    color: white;
}

.title {
    border: 1px solid white;
    text-align: center;
    height: 40px;
    font-size: 30px;
}

.content {
    border: 1px solid white;
    padding: 5px 50px 5px 50px;
}

.key {
    width: 150px;
    float: left;
    text-align: right;
}

.val {
    width: 300px;
}

.sub {
    width: 500px;
}

.footer {
    border: 1px solid white;
    text-align: right;
    font-size: 10px;
}
)";

#endif