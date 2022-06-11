<?php
    $name = $_GET['userId'];//用户名
    $pwd = $_GET['passwd'];//密码
    $json_string = file_get_contents('user.json');
    $data = json_decode($json_string, true);
    //var_dump($json_string); 
    array_push($data,array("userId"=>$name,"passwd"=>$pwd,"priority"=>10));
 
    $json = json_encode($data);
    //var_dump($data);
    file_put_contents('user.json', $json);
    header("Location: ./register.html")
    // class userInfo {
    //     public $userId = "";
    //     public $passwd  = "";
    //     public $priority = 10;
    // }
    // $info = new userInfo();
    // $info->userId = $name;
    // $info->passwd  = $pwd;
    // $json = json_encode($info);
    
    // file_put_contents('user.json', $json);
?>