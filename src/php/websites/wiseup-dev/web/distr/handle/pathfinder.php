<?
//parse url to url1,url2 ...,urlN
$url= $_SERVER['REQUEST_URI'];

//check for /(slash) in the end of the url. if it's missing add it.
if (substr ($url,-1) != '/')$url = $url.'/';
$url   = addslashes (strtolower ($url));
//split url to parts
$urls = explode ("/",$url);
//define constants url1,url1,...,urlN
for($i=1;$i<sizeof($urls) and $i<7;$i++)
{
	if(is_numeric($urls[$i])) $urls[$i]=(int)$urls[$i];
	define('url'.$i,$urls[$i]); 
}

//administration redirect
if(url1== 'admin')
	{
		if(array_key_exists(url2,$main_modules)!== false)
		include_once(adm_engine."/".$main_modules[url2]);
		//else include_once(adm_dir."/main.php");
		include_once(adm_dir."/index.php");
		exit();
	}