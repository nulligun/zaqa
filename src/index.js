import express from 'express';
import Zaqa from "./classes/zaqa";
require( 'trace-unhandled/register' );

const app = express();
const z = new Zaqa();

const bodyParser = require('body-parser');
app.use(bodyParser.json()); // support json encoded bodies
app.use(bodyParser.urlencoded({ extended: true })); // support encoded bodies

z.connect().then(() => {
	console.log("We started");
});

app.post('/afters',  function(req, res){
	const commands = [];
	req.body.payloads.forEach((p) => {
		if ('action' in p) {
			commands.push({cmd: "led.at", args: [p.id, p.r, p.g, p.b]});
		} else {
			commands.push({cmd: "led.after", args: [p.id, p.r, p.g, p.b, p.ar, p.ag, p.ab], url: p.url});
		}
	});
	z.add(commands, function(r) {
		res.send('ok');
	});
});

app.post('/after',  function(req, res){
	const command = {cmd:"led.after", args: [req.body.id, req.body.r, req.body.g, req.body.b, req.body.ar, req.body.ag, req.body.ab], url: req.body.url};
	z.add([command], function(r) {
		res.send('ok');
	});
});

app.get('/at/:id/:r/:g/:b', function(req, res){
	z.add([{cmd:"led.at", args: [req.params.id, req.params.r, req.params.g, req.params.b]}], function(r) {
		res.send('ok');
	});
});

app.get('/at/:id/:r/:g/:b:/:timeout', function(req, res){
	z.add([{cmd: "led.at", args: [req.params.id, req.params.r, req.params.g, req.params.b]}], function(r) {
		res.send('ok');
	});
	setTimeout(function() {
		z.add([{cmd: "led.at", args: [req.params.id, 0, 0, 0]}]);
	}, req.params.timeout * 1000)
});

app.get('/all/:r/:g/:b', function(req, res){
	z.add([{cmd: "led.setAll", args: [req.params.r, req.params.g, req.params.b]}], function(r) {
		res.send('ok');
	});
});

app.get('/all/:r/:g/:b/:timeout', function(req, res){
	z.add([{cmd: "led.setAll", args: [req.params.r, req.params.g, req.params.b]}], function(r) {
		res.send('ok');
	});
	setTimeout(function() {
		z.add([{cmd: "led.setAll", args: [req.params.id, 0, 0, 0]}]);
	}, req.params.timeout * 1000);
});

app.listen(7890);
