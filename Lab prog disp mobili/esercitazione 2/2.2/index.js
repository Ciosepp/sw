"use strict:"
console.log("HI!");



const check(){
	const val = document.getElementById("number");
	console.log("val");

	let x = Math.floor(Math.random());
	console.log("<br>"+x);

	const output = document.getElementById("output");

	if (val == x) output.innerText = "you've been lucky!!"
	else output.innerText = "you'll be lucky next time"
}

const reset(){

}