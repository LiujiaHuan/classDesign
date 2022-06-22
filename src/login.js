let arr = new Array();

//获取json文件 将user和passwd解析
$.getJSON("../user.json", function (data) {
  $.each(data, function (infoIndex, info) {
    let userInfo = {
      userId: "",
      passwd: "",
      priority: 10
    };
    userInfo.userId = info.userId;
    userInfo.passwd = info.passwd;
    userInfo.priority = info.priority;
    arr.push(userInfo);//push

  })

})

var flag = 0;
//登录按钮被按下  触发事件 与从json解析出来的用户作比较
submit_info.onclick = function login() {
  let user = document.getElementsByTagName('input')[0].value;
  let passwd = document.getElementsByTagName('input')[1].value;
  for (j = 0, len = arr.length; j < len; j++) {//遍历
    if (arr[j].userId == user && arr[j].passwd == passwd) {
      //比较成功 进行跳转 本js文件生命周期结束
      window.location.replace(`./login.php?username=${user}&passwd=${passwd}`);
      alert("登陆成功！");
      flag = 1;
    }
  }
  if (flag == 0) {
    alert("账号/密码错误 登录失败");
  }
}