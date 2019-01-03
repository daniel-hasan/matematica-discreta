let arrTime1 = $(".NavContent > div > table > tbody > tr[style='vertical-align: top;'] > td:nth-child(2) > b > a:nth-child(1)");

let arrResultado = $(".NavContent > div > table > tbody > tr[style='vertical-align: top;'] > td:nth-child(3) > b");

let arrTime2 = $(".NavContent > div > table > tbody > tr[style='vertical-align: top;'] > td:nth-child(4) > b > a:nth-child(2)");

for(intI = 0; intI<arrTime2.length ; intI++){
	arrTimeResultado = arrResultado[intI].innerHTML.split("–");
	//console.log(arrTime1[intI].innerHTML+" vs "+arrTime2[intI].innerHTML+": "+arrResultado[intI].innerHTML);
	if(parseInt(arrTimeResultado[0]) > parseInt(arrTimeResultado[1])){
		console.log(arrTime2[intI].innerHTML+", "+arrTime1[intI].innerHTML);
	}else{
		if(parseInt(arrTimeResultado[0]) < parseInt(arrTimeResultado[1])){
			console.log(arrTime1[intI].innerHTML+", "+arrTime2[intI].innerHTML);
		}
	}
	
}
