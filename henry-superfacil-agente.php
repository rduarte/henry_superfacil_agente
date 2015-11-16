<?php
$ch = curl_init();
$login = "http://192.168.10.223/rep.html?pgCode=7&opType=1&lblId=0&lblLogin=rep&lblPass=362190";
curl_setopt($ch, CURLOPT_URL, $login);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
$data = curl_exec ($ch);
curl_setopt($ch, CURLOPT_URL, 'http://192.168.10.223/rep.html?pgCode=8&opType=5&lblId=2&lblDataI=01%2F11%2F15+00%3A00&lblDataF=01%2F12%2F15+00%3A00');
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
$content = curl_exec($ch); 
$data = array("record" => array("name" => "pmt_pacomunicipal.txt", "content" => utf8_encode($content)));   
$data_string = json_encode($data);                                                                             
                                                                                                                     
curl_setopt($ch, CURLOPT_URL, 'http://catador.herokuapp.com/records');                                                                     
curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "POST");                                                                     
curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);                                                                  
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);                                                                      
curl_setopt($ch, CURLOPT_HTTPHEADER, array(                                                                          
    'Content-Type: application/json',                                                                                
    'Content-Length: ' . strlen($data_string))                                                                       
);                                                                                                                   
                                                                                                                     
$result = curl_exec($ch);
  
curl_close ($ch);
?>
