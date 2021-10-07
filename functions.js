/* Fonction d'inscription et de connexion */

function Register(){
    username = document.getElementById('username');
    password = document.getElementById('password');

    var mysql = require('mysql');
    var con = mysql.createConnection({

    host: "192.168.64.52",
    user: "root",
    password: "root",
    database: "Project_QT"
    });

    request = con.query("SELECT COUNT(*) FROM Users WHERE username =" + username); 

    console.log(username + " / " + password);
    
}

/* connexion à la BDD */

var mysql = require('mysql');
    var con = mysql.createConnection({

    host: "192.168.64.52",
    user: "root",
    password: "root",
    database: "Project_QT"
    });

function debug(message) {
    debugTextArea.value += message + "\n";
    debugTextArea.scrollTop = debugTextArea.scrollHeight;
}

/* Fonction qui récupère les messages */

function sendMessage() {
    var nickname = document.getElementById("inputNick").innerText;
    var msg = document.getElementById("inputText").value;
    var strToSend = nickname + ": " + msg;
    if ( websocket != null )
    {
        document.getElementById("inputText").value = "";
        websocket.send( strToSend );
        console.log( "string sent :", '"'+strToSend+'"' );
        debug(strToSend);
    }
}

/* Affichage des messages */

function initWebSocket() {
    try {
        if (typeof MozWebSocket == 'function')
            WebSocket = MozWebSocket;
        if ( websocket && websocket.readyState == 1 )
            websocket.close();
        var wsUri = "ws://" + document.getElementById("webSocketHost").value;
        websocket = new WebSocket( wsUri );
        websocket.onopen = function (evt) {
            debug("Connexion");
            document.getElementById("disconnectButton").disabled = false;
            document.getElementById("sendButton").disabled = false;
        };
        websocket.onclose = function (evt) {
            debug("Déconnexion");
            document.getElementById("disconnectButton").disabled = true;
            document.getElementById("sendButton").disabled = true;
        };
        websocket.onmessage = function (evt) {
            console.log( "Message received :", evt.data );
            debug( evt.data );
        };
        websocket.onerror = function (evt) {
            debug('ERROR: ' + evt.data);
        };
    } catch (exception) {
        debug('ERROR: ' + exception);
    }
}

/* Arrêt du Websocket*/

function stopWebSocket() {
    if (websocket)
        websocket.close();
}