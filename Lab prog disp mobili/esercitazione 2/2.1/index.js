"use strict:"
console.log("HI!");

const printText = () => {

	const input = document.getElementById("text");
	console.log(input.value);

	const output = document.getElementById("output");
	output.innerText = input.value;
}