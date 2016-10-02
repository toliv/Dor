var express = require('express');
var bodyparser = require('body-parser');
var pg = require('pg');
var app = express();
var twilio = require('twilio');
var client = new twilio.RestClient(process.env.TW_SID, process.env.TW_AUTH);

app.use(bodyparser.json())

app.set('port', (process.env.PORT || 5000));

app.use(express.static(__dirname + '/public'));

// views is directory for all template files
app.set('views', __dirname + '/views');
app.set('view engine', 'ejs');

app.get('/', function(request, response) {
	response.render('pages/index');
});

function setUpDb(){
	pg.connect(process.env.DATABASE_URL, function(err, pgclient, done) {
		pgclient.query(
			"CREATE TABLE IF NOT EXISTS guests (" +
			"id SERIAL PRIMARY KEY, "+ 
			"name TEXT NOT NULL, " + 
			"number TEXT NOT NULL, " +
			"password TEXT NOT NULL UNIQUE" + 
			"permanent BOOLEAN NOT NULL DEFAULT FALSE" + 
			");",
			function(err, result) {
				console.log("MADE A TABLE")
				done();
			});
	});
};

app.get('/api/v1/guests', function(req, resp){
	pg.connect(process.env.DATABASE_URL, function(err, pgclient, done) {
		pgclient.query("SELECT * FROM guests;",
			function(err, result) {
				if(err){
					resp.status(400).json({'err': err.detail});
					console.error(err);
				} else {
					console.log('Got our guests!');
					resp.json(result.rows);
				}
			});
	});
});

app.delete('/api/v1/guests', function(req, resp){
	console.log('Deleting id', req.query.id);
	pg.connect(process.env.DATABASE_URL, function(err, pgclient, done) {
		pgclient.query("DELETE FROM guests WHERE id = $1;", [req.query.id],
			function(err, result) {
				if(err){
					resp.status(400).json({'err': err.detail});
					console.error(err);
				} else {
					console.log('Deleted a guest. Chillllll');
					resp.sendStatus(200);
				}
			});
	});
});

app.put('/api/v1/guests', function(req, resp){
	console.log('Updating id', req.body.id);
	pg.connect(process.env.DATABASE_URL, function(err, pgclient, done) {
		pgclient.query("UPDATE guests SET permanent = $2 WHERE id = $1;", [req.body.id, req.body.permanent],
			function(err, result) {
				if(err){
					resp.status(400).json({'err': err.detail});
					console.error(err);
				} else {
					console.log('Updated a guest. Chillllll');
					resp.sendStatus(200);
				}
			});
	});
});


app.post('/api/v1/guests', function(req, resp){
	pg.connect(process.env.DATABASE_URL, function(err, pgclient, done) {
		pgclient.query("INSERT INTO guests(name, number, password) VALUES ($1, $2, $3);",
			[req.body.name, req.body.number, req.body.password],
			function(err, result) {
				if(err){
					resp.status(500).json(err);
					console.error(err);
				} else {
					console.log('Created a new guest. Chillllll');
					resp.sendStatus(200);
					client.sendMessage({
						to: req.body.number,
						from: process.env.TW_NUMBER, 
						body: 'Hey, '+ req.body.name + '. You\'ve been invited to a party! Your password is '
						 + req.body.password
					}, 
					function(err, responseData) {
						if (err) { console.error(err);}
					});
				}
			});
	});
});

app.post('/api/v1/verify', function(req, resp){
	var pass = req.body.pass;
	console.log("Password entered", pass);
	pg.connect(process.env.DATABASE_URL, function(err, pgclient, done) {
		pgclient.query("SELECT name,id,permanent FROM guests WHERE password = '" + pass + "';", function(err, result) {
			if (err){ 
				console.error(err);
				resp.sendStatus(400); 
			}
			else if(result.rows.length == 0){
				resp.sendStatus(400);
			} else {
				resp.sendStatus(200);
				if(!result.rows[0].permanent){
					pgclient.query("DELETE FROM guests WHERE id = $1;", [result.rows[0].id]);
				}

				client.sendMessage({
					to: process.env.YOUR_NUMBER,
					from: process.env.TW_NUMBER, 
					body: 'Your friend ' + result.rows[0].name + ' is here.'
				}, 
				function(err, responseData) {
					if (err) { console.error(err);}
					done();
				});
			}
		});
	});
});

app.listen(app.get('port'), function() {
	setUpDb();
	console.log('Node app is running on port', app.get('port'));
});


