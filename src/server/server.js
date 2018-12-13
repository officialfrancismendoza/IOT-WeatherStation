/*
  Programmer: Francis Mendoza
  Date: 11/27/18, 18:40 hours MST
  Purpose: Calculate average
*/

//Nodemailer functionality
var nodemailer = require('nodemailer');
//let transporter = nodemailer.createTransport('smtp://ame394fall2018%40gmail.com:francissamuelmendoza7@gmail.com');
let transporter = nodemailer.createTransport('smtp://ame394fall2018%40gmail.com:nodemcu1234@smtp.gmail.com');
//-----------------------------------------------------------------------------------------------------
var MS = require("mongoskin");
var express = require("express");
var app = express();
var bodyParser = require('body-parser');
var errorHandler = require('errorhandler');
var methodOverride = require('method-override');
var hostname = process.env.HOSTNAME || 'localhost';
var port = 1234;
var VALUEt = 0;
var VALUEh = 0;
var VALUEtime = 0;
var tempCheck = 0;
//-----------------------------------------------------------------------------------------------------
//var db = MS.db("mongodb://root:OfaZ13Q6DERS@localhost:27017/sensorData");
var db = MS.db("mongodb://root:OfaZ13Q6DERS@localhost:27017/sensorData");
app.get("/", function (req, res)
{
    res.redirect("/index.html");
});
//-----------------------------------------------------------------------------------------------------
//FUNCTION: getAverage
app.get("/getAverage", function (req, res)
{
  //res.writeHead(200, {'Content-Type': 'text/plain'});
  var from = parseInt(req.query.from);
  var to = parseInt(req.query.to);

  //MONGODB
  db.collection("data").find({time:{$gt:from, $lt:to}}).toArray(function(err, result){
  	console.log(err);
  	console.log(result);
  	var tempSum = 0;
  	var humSum = 0;
  	for(var i=0; i< result.length; i++){
  		tempSum += result[i].t || 0;
  		humSum += result[i].h || 0;
  	}
  	var tAvg = tempSum/result.length;
  	var hAvg = humSum/result.length;
    res.send(tAvg.toString() + " " + hAvg.toString() + " " + from.toString() + "\r");
  });

});
//-----------------------------------------------------------------------------------------------------
//FUNCTION: getLatest
app.get("/getLatest", function (req, res)
{
    db.collection("data").find({}).sort({time:-1}).limit(10).toArray(function(err, result){
        res.send(JSON.stringify(result));
    });
});
//-----------------------------------------------------------------------------------------------------
//FUNCTION: getData
app.get("/getData", function (req, res)
{
    var from = parseInt(req.query.from);
    var to = parseInt(req.query.to);
    db.collection("data").find({time:{$gt:from, $lt:to}}).sort({time:-1}).toArray(function(err, result){
        res.send(JSON.stringify(result));
    });
});
//-----------------------------------------------------------------------------------------------------
//FUNCTION: getValue
app.get("/getValue", function (req, res)
{
  //res.writeHead(200, {'Content-Type': 'text/plain'});
  res.send(VALUEt.toString() + " " + VALUEh + " " + VALUEtime + "\r");
});
//-----------------------------------------------------------------------------------------------------
//FUNCTION: setValue
app.get("/setValue", function (req, res)
{
  VALUEt = parseFloat(req.query.t);
  VALUEh = parseFloat(req.query.h);
  VALUEtime = new Date().getTime();
	var dataObj =
  {
		t: VALUEt,
		h: VALUEh,
		time: VALUEtime
	};

  //Condition for sensor to trip after 100*F
	if(VALUEt > 10)
  {
        var date = new Date(); // get the current date.
        if(date.getTime() >= (tempCheck + 300000))
        { //Condition to send after 5 minutes
            tempCheck = date.getTime();
            sendEmail(VALUEt, date);
		    }
	}
	db.collection("data").insert(dataObj, function(err,result){
		console.log("added data: " + JSON.stringify(dataObj));
	});
  res.send(VALUEtime.toString());
});
//-----------------------------------------------------------------------------------------------------
//FUNCTION: sendEmail
function sendEmail(temp, time)
{
  let message =
  {
      // Comma separated list of recipients
      to: 'Francis Mendoza <francissamuelmendoza7@gmail.com>',
      subject: 'IoT Sensor Alert- Warning',
      // plaintext body

      //MODIFY TO GET REAL TIME STATEMENT
      text: 'Sensor tripped at ' + time + '. Temperature exceeded 100*F. Current Temperature: ' + temp,
      // HTML body

      //Can view as plain text HTML
      html:  '<p> Sensor tripped at ' + time + '. Temperature exceeded 100*F. Current Temperature: ' + temp + '</p>',
      watchHtml:  '<p> Sensor tripped at ' + time + '. Temperature exceeded 100*F. Current Temperature: ' + temp + '</p>'
    }


    console.log('Sending Mail');
    transporter.sendMail(message, function(err, result)
    {
        console.log(err,result);
    });
}
//-----------------------------------------------------------------------------------------------------
//ASSORTED FUNCTIONS
app.use(methodOverride());
app.use(bodyParser());
app.use(express.static(__dirname + '/public'));
app.use(errorHandler());

console.log("Simple static server listening at http://" + hostname + ":" + port);
app.listen(port);
