	let myButton = document.querySelector("#myButton"); 
	myButton.addEventListener("click", doSomething, false);
	let counter = 0;
	function doSomething(e) 
	{
		counter++;
		console.log("Button clicked " + counter + " times!");
	
		if (counter == 3) 
		{
			showMore();
		}
		function showMore() 
		{
			console.warn("This is a warning!");
			console.error("This is an error!");
		}
	}


	let button = document.querySelector("#myButton");
	button.addEventListener("mouseover", hovered, false);
	button.addEventListener("mouseout", hoveredOut, false);
	function hovered(e) {
	console.log("Hovered!");
	}
	function hoveredOut(e) {
	console.log("Hovered Away!");
	}