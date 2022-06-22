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


<<<<<<< HEAD:src/register.php
    $path = "../log.txt" ;
=======
    $path = "./log.txt" ;
>>>>>>> b175b0198df5cb79f1c136162c92b1afdc0be83b:register.php
    $split = "-";
    $flag = "r";
    $json = $flag.$split.$name.$split.$pwd;
    file_put_contents($path, $json);
    header("Location: ./register.html")

?>