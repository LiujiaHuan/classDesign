var url = top.window.location.href;
var u = url.split("?");
var user = u[1];
//alert(`hello! ${user}!!! Take care of yourself and be well!`);
var jsonPath = `./jsonFile/${user}.json`

let arr =  new Array()
$.getJSON(jsonPath, function(data) {
    //console.log(data);
    //存储数据的变量 
    //var strHtml = "";
            //清空内容 
    $.each(data, function(infoIndex, info) {
        let userAffair={
            time :"",
            affair :""
        };
        userAffair.time=info.time;
        userAffair.affair=info.affair;
        arr.push(userAffair);
    })

    for(j = 0,len=arr.length; j < len; j++) {
       // alert(arr[j].affair);
        let div = document.createElement('div');
        div.innerHTML = `time :${arr[j].time} affair:${arr[j].affair}`;
        document.body.append(div);
    }
})



