let arr = new Array();
$.getJSON("./user.json", function(data) {
    //console.log(data);
    //存储数据的变量 
    //var strHtml = "";
            //清空内容 
    $.each(data, function(infoIndex, info) {
      let userInfo={
        userId :"",
        passwd :"",
        priority:10
      };
      userInfo.userId=info.userId;
      userInfo.passwd=info.passwd;
      userInfo.priority=info.priority;
      arr.push(userInfo);

    })

})
submit_info.onclick = function login(){
  let user = document.getElementsByTagName('input')[0].value;
  let passwd = document.getElementsByTagName('input')[1].value;
  for(j = 0,len=arr.length; j < len; j++) {
    if(arr[j].userId == user && arr[j].passwd == passwd){
      window.location.replace(`./login.php?username=${user}&passwd=${passwd}`);
      alert("登陆成功！")
    }
 }
}