<?php
    $time = $_GET['time'];//用户名
    $affair = $_GET['affair'];//密码
   
    $url = parse_url($_SERVER['HTTP_REFERER']);
    $user = $url[query];

    $path = "./jsonFile/{$user}.json" ;
    $json_string = file_get_contents($path);
    //echo $path;
    $data = json_decode($json_string, true);
    //var_dump($data); 
    array_push($data,array("time"=>$time,"affair"=>$affair));
   // var_dump($data);
    $json = json_encode($data);
  //  var_dump($json);
    file_put_contents($path, $json);


?>