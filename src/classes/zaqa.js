import axios from 'axios';
import Focus from "@chrysalis-api/focus";
import { Model01 } from "@chrysalis-api/hardware-keyboardio-model01";
const config = require('config');
const open = require('opn');

export default class Zaqa {
	constructor()
	{
		this.allCommands = [];
		this.urls = {};
		this.processing = false;

		const self = this;
		this.focus = new Focus({egress_callback: function(data) {
			self.keypressClassback(data);
		}});

	}

	connect() {
		return new Promise((resolve, reject) => {
			console.log("Opening keyboard");
			this.focus.open(Model01).then((port) => {
				console.log("Done opening Keyboard");
				resolve();
			}).catch((e) => {
				console.log("Problem opening keyboard");
				reject();
			});
		});
	}

	add(commands, callback) {
		this.allCommands.push({commands: commands, callback: callback});
		if (!this.processing) {
			this.processing = true;
			this.process();
		}
	}

	async doCommand(theCommand) {
		const commands = theCommand.commands;
		let result = false;
		let sendFailure = false;
		for (const cmd of commands) {
			if ('url' in cmd) {
				this.set(cmd.args[0], cmd.url);
			}
			try {
				await this.focus.command(cmd.cmd, ...cmd.args);
				result = true;
			} catch (e) {
				sendFailure = true;
				result = false;
				console.log("Could not send command");
				break;
			}
		}

		if (result) {
			if ('callback' in theCommand) {
				theCommand.callback("ok");
			}
		} else {
			if (sendFailure) {
				try {
					console.log("Send failure, attempting to reconnect");
					await this.connect();
					console.log("We reconnected!");
				} catch (e) {
					console.log("Could not reconnect");
				}
			}
		}

		return result;
	}

	async process()
	{
		const theCommand = this.allCommands.shift();

		const result = await this.doCommand(theCommand);

		if (!result) {
			this.allCommands.push(theCommand);
		}

		const self = this;
		if (this.allCommands.length > 0) {
			setTimeout(function() {
				self.process();
			}, 500);
		} else {
			this.processing = false;
		}
	}

	set(index, url) {
		this.urls[index] = url;
	}

	keypressClassback(data)
	{
		const cmd = data.split(' ');
		if (cmd[0] === "ackall")
		{
			config.get('app.ackall_endpoints').forEach((e) => {
				axios.get(e).then((res) => {
					console.log(res.data);
					console.log("called ackall endpoint " + e);
				}).catch((err) => {
					console.log(err);
					console.log("error calling ackall endpoint " + e);
				});
			});
		}
		else if (cmd[0] === "ack")
		{
			config.get('app.ack_endpoints').forEach((e) => {
				const url = this.urls[cmd[1]];
				console.log("Key pressed: " + cmd[1] + " url: " + url);
				if (url) {
					open(url);
				}
			});
		}
		console.log("ZAQ: " + data);
	}
}
