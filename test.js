$.getJSON("./test.json", function(data) {
    //console.log(data);
    //存储数据的变量 
    //var strHtml = "";
            //清空内容 
    let arr = new Array()

    $.each(data, function(infoIndex, info) {
      strHtml += "用户名：" + info["userId"] + "<br>";
      strHtml += "密码：" + info["passwd"] + "<br>";
      strHtml += "权限：" + info["priority"] + "<br>";
      strHtml += "<hr>"
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
    //显示处理后的数据 
    // $jsontip.html(strHtml);
    //console.log(arr[0].userId);
})
//当登录键被按下，进行用户信息比对
submit_info.onclick = function login(){
    let user = document.getElementsByTagName('input')[0].value;
    let passwd = document.getElementsByTagName('input')[1].value;
    //console.log(user,passwd);
}