<?php
header("content-type:text/plain; charset=utf-8");

$basedir ="./datas";

//filecontent, filelist
$fun =$_GET["fun"];
if( $fun == "")
	$fun =$_POST["fun"];
$uri =$_GET["path"];
if( $uri == "")
	$uri =$_POST["path"];
$ext =$_GET["fext"];
if( $ext == "")
	$ext =$_POST["fext"];
	
function get_txtfilecontent( $base, $path)
{
	$fpath =$base.$path;
	
	if( !file_exists( $fpath))
		return "";

	$ret =file_get_contents( $fpath);
	
	//$fc =base64_encode($ret);
	
	//$ret ="<c>".$ret."</c>";
	
	return $ret;
}

function get_filelist( $base, $path, $ext)
{
	$ret ="";
		
	$fdir =$base.$path;
	
	if( !is_dir( $fdir))
		return $ret;
		
	$fh =opendir( $fdir."/.");
	
	if( $fh == 0)
		return $ret;
		
	while( ( $f =readdir( $fh)) != false)
	{
		if( $f == "." || $f == "..")
			continue;

		$fullf =$fdir."/".$f;

		if( !is_file( $fullf))
			continue;
		
		$fe =pathinfo($fullf, PATHINFO_EXTENSION);

		if( strlen($fe) > 0)
			$fe =".".$fe;
		
		if( $fe != $ext)
			continue;

		$ret =$ret.$f.";";
	}
	
	closedir( $fh);

	$ret ="<c>".$ret."</c>";

	return $ret;
}

if( $fun == "filecontent")
{
	echo get_txtfilecontent( $basedir, $uri);
}
else if( $fun == "filelist")
{
	echo get_filelist( $basedir, $uri, $ext);
}
else
{
	echo '';
}

?>