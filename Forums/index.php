<?php

// The session is necessary for many things.
session_start();

// MySQL variables.
$MySQL['Host'] = "127.0.0.1";
$MySQL['Username'] = "root";
$MySQL['Password'] = "";
$MySQL['Connection'] = mysql_connect($MySQL['Host'], $MySQL['Username'], $MySQL['Password']);
$MySQL['SelectDB'] = mysql_select_db("forums");

// TODO: Database class.
class DB
{
	public static $Host = null;
	public static $Username = null;
	public static $Password = null;
	public static $Connection = null;
}

// Some server and IP variables.
$ServerName = $_SERVER['SERVER_NAME'];
$ServerIP = $_SERVER['SERVER_ADDR'];
$UserIP = $_SERVER['REMOTE_ADDR'];

// Various settings. Statically defined for ease of implementation.
$MaxThreadTitleLength = 50;
$MaxPostLength = 20000;
$PostFloodTimeout = 20;
$PostsPerPage = 15;
$ThreadsPerPage = 20;

class PostType
{
	public static $NewThread = 1;
	public static $NewPost = 2;
}

class PermissionType
{
	public static $ViewForum = "ViewForum";
	public static $ViewThreads = "ViewThreads";
	public static $CreateThreads = "CreateThreads";
	public static $Post = "Post";
}

$UserGroups = array( 0 => "User", 1 => "Administrator" );

function UserGroupNameFromID($id)
{
	global $UserGroups;
	return $UserGroups[$id];
}

function UserGroupIDFromName($name)
{
	global $UserGroups;
	$keys = array_keys($UserGroups);
	for ($i = 0; $i < count($UserGroups); $i++)
	{
		if ($UserGroups[$i] == $name)
		{
			return $keys[$i];
		}
	}
	return -1;
}

// The page title variable.
$PageTitle = "CXBoard";
// This variable will hold all the predetermined page content.
$PageContent = "";

// Array of strings for breadcrumb formation.
$Breadcrumbs = array();

function addBreadcrumb($content)
{
	global $Breadcrumbs;
	$Breadcrumbs[count($Breadcrumbs)] = $content;
}

addBreadcrumb("<a href='?p=home'>Forum Home</a>");

function redirect($NewURL, $Seconds = 0)
{
	echo "<meta http-equiv='refresh' content='$Seconds;url=$NewURL'>";
}

function mysql_fetch_table($theQuery)
{
	$ret = array();
	$query = mysql_query($theQuery);
	$row = mysql_fetch_array($query);
	$i = 0;
	while ($row != false)
	{
		$ret[$i] = $row;
		$i++;
		$row = mysql_fetch_array($query);
	}
	return $ret;
}

class PageEntry
{
	public $Name = null;
	public $PageName = null;
	public $ShowLoggedIn = null;
	public $ShowLoggedOut = null;
	
	public function __construct($_Name, $_PageName, $_ShowLoggedIn, $_ShowLoggedOut)
	{
		$this->Name = $_Name;
		$this->PageName = $_PageName;
		$this->ShowLoggedIn = $_ShowLoggedIn;
		$this->ShowLoggedOut = $_ShowLoggedOut;
	}
	
	public function IsVisible()
	{
		return (User::IsLoggedIn() == $this->ShowLoggedIn || !User::IsLoggedIn() == $this->ShowLoggedOut);
	}
}

$Pages = array(	new PageEntry("Home", "home", true, true),
				new PageEntry("Login", "login", false, true),
				new PageEntry("Logout", "logout", true, false)
				);
$NumPages = count($Pages);

function GetLastVisiblePage()
{
	global $NumPages, $Pages;
	static $ret = -1;
	if ($ret == -1)
	{
		$ret = 0;
		for ($i = 0; $i < $NumPages; $i++)
		{
			if ($Pages[$i]->IsVisible())
			{
				$ret = $i;
			}
		}
	}
	return $ret;
}

function MakeNewPost($ThreadID, $PostContent, $ForumID = null)
{
	// Parse the post for storage.
	$ParsedPostContent = $PostContent;
	$ParsedPostContent = str_replace("\r\n", "\n", $ParsedPostContent);
	$ParsedPostContent = str_replace("\r", "\n", $ParsedPostContent);
	
	// Store the post and make necessary alterations to other related data.
	$Time = time();
	mysql_query("INSERT INTO `posts` (`ThreadID`, `PosterID`, `PostContent`, `PostTime`) VALUES ('" . $ThreadID . "', '" . $_SESSION['UserID'] . "', '" . $ParsedPostContent . "', '" . $Time . "')");
	$ThePostID = mysql_insert_id();
	mysql_query("UPDATE `users` SET `PostCount`=`PostCount`+'1', `LastPostTime`='" . $Time . "' WHERE `ID`='" . $_SESSION['UserID'] . "'");
	mysql_query("UPDATE `threads` SET `PostCount`=`PostCount`+'1', `LastPostID`='" . $ThePostID . "', `LastPostTime`='" . $Time . "' WHERE `ID`='" . $ThreadID . "'");
	if ($ForumID == null)
	{
		$ForumID = mysql_fetch_assoc(mysql_query("SELECT `ForumID` FROM `threads` WHERE `ID`='" . $ThreadID . "'"));
		$ForumID = $ForumID["ForumID"];
	}
	mysql_query("UPDATE `forums` SET `PostCount`=`PostCount`+'1', `LastPostID`='" . $ThePostID . "' WHERE `ID`='" . $ForumID . "'");
	return $ThePostID;
}

function MakeNewThread($ForumID, $Name, $PostContent)
{
	mysql_query("INSERT INTO `threads` (`Name`, `ForumID`) VALUES ('" . $Name . "', '" . $ForumID . "')");
	$ThreadID = mysql_insert_id();
	$PostID = MakeNewPost($ThreadID, $PostContent, $ForumID);
	mysql_query("UPDATE `threads` SET `FirstPostID`='" . $PostID . "' WHERE `ID`='" . $ThreadID . "'");
	mysql_query("UPDATE `forums` SET `ThreadCount`=`ThreadCount`+'1' WHERE `ID`='" . $ForumID . "'");
	return $ThreadID;
}

function GetPostByID($id)
{
	return mysql_fetch_array(mysql_query("SELECT * FROM `posts` WHERE `ID`='$id'"));
}

function GetThreadByID($id)
{
	return mysql_fetch_array(mysql_query("SELECT * FROM `threads` WHERE `ID`='$id'"));
}

function GetForumByID($id)
{
	return mysql_fetch_array(mysql_query("SELECT * FROM `forums` WHERE `ID`='$id'"));
}

function GetUserByID($id)
{
	return mysql_fetch_array(mysql_query("SELECT * FROM `users` WHERE `ID`='$id'"));
}

function GetMySQLVersion()
{
	$derp = mysql_fetch_array(mysql_query("SELECT version();"));
	$derp = $derp['version()'];
	return $derp;
}

function PrintArray($array)
{
	print "<table border='1'>";
	foreach ($array as $key => $value)
	{
		print "<tr><td>" . $key . "</td><td>" . $value . "</td></tr>";
	}
	print "</table>";
}

function RegisterUser($Username, $Password)
{
	global $UserIP;
	mysql_query("INSERT INTO `users` (`Username`, `Password`, `LastIP`, `RegistrationIP`) VALUES ('".$Username."', '".$Password."', '".$UserIP."', '".$UserIP."');");
}

class User
{
	public static $Info = null;
	public static $LoggedIn = null;
	
	public static function Init()
	{
		User::GetLoggedInStatus();
		User::FetchInfo();
	}
	
	public static function GetLoggedInStatus()
	{
		User::$LoggedIn = isset($_SESSION['UserID']);
	}
	
	public static function IsLoggedIn()
	{
		return User::$LoggedIn;
	}
	
	public static function FetchInfo()
	{
		if (User::IsLoggedIn())
		{
			User::$Info = GetUserByID($_SESSION['UserID']);
		}
	}
	
	public static function Login($Username, $Password)
	{
		if (User::IsLoggedIn())
		{
			return -1;
		}
		
		User::$Info = mysql_fetch_array(mysql_query("SELECT * FROM `users` WHERE `Username`='" . $Username . "' AND `Password`='" . $Password . "'"));
		
		if (User::$Info != null)
		{
			$_SESSION['UserID']		= User::$Info['ID'];
			$_SESSION['Username']	= User::$Info['Username'];
			$_SESSION['Password']	= User::$Info['Password'];
			
			mysql_query("UPDATE `users` SET `LastLogin`='" . time() . "' WHERE `ID`='" . User::$Info['ID'] . "'");
		}
		
		User::GetLoggedInStatus();
		return User::IsLoggedIn();
	}
	
	public static function Logout()
	{
		$Condition = User::IsLoggedIn();
		if ($Condition)
		{
			unset($_SESSION['UserID']);
			unset($_SESSION['Username']);
			unset($_SESSION['Password']);
		}
		User::GetLoggedInStatus();
		return $Condition;
	}
	
	public static function HasPermission($ForumArray, $RequestedPermission)
	{
		$AllAllowedGroups = explode("|", $ForumArray['Permissions[' . $RequestedPermission . ']']);
		
		for ($i = 0; $i < count($AllAllowedGroups); $i++)
		{
			if (intval($AllAllowedGroups[$i]) == intval(User::$Info['UserGroup']))
			{
				return true;
			}
		}
		return false;
	}
};

// Initialize the user information class.
User::Init();

function generate_page_listing($showingOffset, $lastShownOfOffset, $totalItems, $totalPages, $currentPage, $currentID, $pageName)
{
	$retVal = "Showing " . (($totalItems > 0) ? $showingOffset+1 : 0) . " to " . ($lastShownOfOffset <= $totalItems ? $lastShownOfOffset : $totalItems) . " of " . $totalItems . " [Page " . $currentPage . " of " . $totalPages . "] ";

	// [Page # of T#] [1][2][3][4]-[10] ... [>][Last>>]
	if ($currentPage > 3 && $totalPages > 5)
	{
		$retVal .= "[<a href='?p=" . $pageName . "&id=" . $currentID . "&page=1'>« First</a>]";
		$retVal .= "[<a href='?p=" . $pageName . "&id=" . $currentID . "&page=" . ($currentPage - 1) . "'>«</a>]";
		$retVal .= " .. ";
	}
	
	$LowestDisplayedPage = ($currentPage < 3 ? 1 : $currentPage - 2);
	$HighestDisplayedPage = (($currentPage < ($totalPages - 2)) ? $currentPage + 2 : $totalPages);
	
	// Adjust page showing range so there are always at least 5 pages visible.
	if ($totalPages > 5)
	{
		$LowestDisplayedPage -= (($currentPage > ($totalPages - 2)) ? ($currentPage - ($totalPages - 2)) : 0);
		$HighestDisplayedPage += (($currentPage < 3) ? (3 - $currentPage) : 0);
	}

	for ($currPgCount = $LowestDisplayedPage; $currPgCount <= $HighestDisplayedPage; $currPgCount++)
	{
		$currPageContent = (string)$currPgCount;
		if ($currPgCount != $currentPage)
		{
			$currPageContent = "<a href='?p=" . $pageName . "&id=" . $currentID . "&page=" . $currPgCount . "'>" . $currPageContent . "</a>";
		}
		else
		{
			$currPageContent = "<b>" . $currPageContent . "</b>";
		}
		$retVal .= "[" . $currPageContent . "]";
		if ($currPgCount < $totalPages)
		{
			$retVal .= " ";
		}
	}
	
	if ($currentPage < ($totalPages - 2) && $totalPages > 5)
	{
		$retVal .= " .. ";
		$retVal .= "[<a href='?p=" . $pageName . "&id=" . $currentID . "&page=" . ($currentPage + 1) . "'>»</a>]";
		$retVal .= "[<a href='?p=" . $pageName . "&id=" . $currentID . "&page=" . $totalPages . "'>Last »</a>]";
	}
	
	return $retVal;
}

function PrintHeader()
{
	global $PageTitle;
	echo "
<html>
	<head>
		<title>" . $PageTitle . "</title>
		<style>
		body
		{
			margin: 0;
			padding: 0;
			background-color: #222222;
		}
		div
		{
			box-sizing: border-box;
			-moz-box-sizing: border-box;
			-ms-box-sizing: border-box;
			-webkit-box-sizing: content-box;
		}
		.everythingcontainer
		{
			width: 95%;
			margin: 0 auto;
			background-color: #FFFFFF;
			padding-left: 10px;
			padding-right: 10px;
		}
		.subcontentlight
		{
			background: #E9E9E9;
			color: #333333;
		}
		.subcontentdark
		{
			background: #DBDBDB;
			color: #333333;
		}
		.subcontentcontainer
		{
			background: #FFFFFF;
			color: #333333;
			border: 1px solid #CCCCCC;
			/* border-spacing: 1px; */
		}
		.herpderpuseless
		{
			width: 750px;
			margin: 0 auto;
			background-color: #111111;
			/* border: 1px solid #FF006E; */
			padding-left: 50px;
			padding-right: 50px;
			/* height: 100%; */
			border-left: 1px solid #FF006E;
			border-right: 1px solid #FF006E;
		}
		.majorheader
		{
			background: #19579E url('catbg.gif') repeat-x top left;
			color: white;
			font: bold 10pt verdana, geneva, lucida, 'lucida grande', arial, helvetica, sans-serif;
		}
		.majorheader a:link, .majorheader a:visited
		{
			color: #FFFFFF;
			text-decoration: none;
			border-bottom: 1px dotted #FFFFFF;
		}
		.majorheader a:hover, .majorheader a:active
		{
			color: #FFFFFF;
			text-decoration: none;
			border-bottom: 1px solid #FFFFFF;
		}
		.minorheader
		{
			background: #222222 url('subcatbg.gif') repeat-x top left;
			color: white;
			font-weight: bold;
		}
		.minorheader a:link, .minorheader a:visited
		{
			color: #FFFFFF;
			text-decoration: none;
			border-bottom: 1px dotted #FFFFFF;
		}
		.minorheader a:hover, .minorheader a:active
		{
			color: #FFFFFF;
			text-decoration: none;
			border-bottom: 1px solid #FFFFFF;
		}
		.navbar
		{
			font: 11px verdana, geneva, lucida, 'lucida grande', arial, helvetica, sans-serif;
		}
		/* ***** small padding on 'thead' elements ***** */
		td.thead, th.thead, div.thead { padding: 4px; }
		td,tr,p,br,input,div
		{
			color: #FF006E;
			font-family: 'Segoe UI';
			font-size: 7pt;
		}
		a:link
		{
			color: #19579E;
			text-decoration: none;
		}
		a:visited
		{
			color: #19579E;
			text-decoration: none;
		}
		a:hover, a:active
		{
			color: #000000;
			text-decoration: underline;
		}
		form
		{
			padding: 0px;
			margin: 0px;
		}
		</style>
	</head>
	
	<body>
		<div class='everythingcontainer'>
			<br />
					<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%' height='100px'>
						<tr><td class='subcontentdark'>Logo</td></tr>
					</table>
					<br />
					<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%' height='30px'>
						<tr><td class='minorheader'>";
						
						global $NumPages, $Pages;
						for ($i = 0; $i < $NumPages; $i++)
						{
							if ($Pages[$i]->IsVisible())
							{
								echo "<a href='?p=" . $Pages[$i]->PageName . "'>" .$Pages[$i]->Name . "</a>";
								if ($i < GetLastVisiblePage())
								{
									echo " | ";
								}
							}
						}
						
						echo "</td></tr>
					</table>
					<br />
					<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'><tr>
						<td class='subcontentlight'>";
						
						global $Breadcrumbs;
						$NumBreadcrumbs = count($Breadcrumbs);
						for ($i = 0; $i < $NumBreadcrumbs; $i++)
						{
							echo $Breadcrumbs[$i];
							if ($i + 1 < $NumBreadcrumbs)
							{
								echo " » ";
							}
						}
						
						echo "</td>
						<td class='subcontentlight' style='padding: 0px;' width='250px'>
						<form method='post' action='?p=login'>
							<table border='0' align='center'>";
							if (User::IsLoggedIn())
							{
								echo "<tr><td>Welcome, " . User::$Info["Username"] . ".<br />You have 0 unread PMs.</td></tr>";
							}
							else
							{
								echo "
								<tr><td>Username:&nbsp;</td><td><input size='10' type='text' name='Username'></input></td><td><input type='checkbox'>Remember Me</input></td></tr>
								<tr><td>Password:&nbsp;</td><td><input size='10' type='password' name='Password'></input></td><td><input type='submit' value='Login' name='Login'></input></td></tr>";
							}
							echo "
							</table>
						</form>
						</td>
					</tr>
					<tr><td style='padding: 3px;' class='majorheader navbar' colspan='2'><b>Register</b></td></tr>
					</table>
					<br />
					<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'>
						<tr><td class='majorheader'>Welcome trololol</td></tr>
						<tr><td class='subcontentlight'>Yeah, this is a troll message. okay whatever.</td></tr>
					</table>
					<br />
";
}

function PrintFooter()
{
	echo "
					<br />
					<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'>
						<tr><td class='minorheader'>Copyright &copy; 2011 ClericX. All rights reserved. « »</td></tr>
					</table>
			<br />
		</div>
	</body>
</html>
";
}

// Default to "home" if p is not specified or blank.
if (!isset($_GET['p']) || $_GET['p'] == "")
{
	$_GET['p'] = "home";
}

switch ($_GET['p'])
{
	case 'home':
	{
		$PageContent .= "
					<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'>
						<tr><td class='minorheader thead'>&nbsp;</td><td class='minorheader thead'>Forum</td><td class='minorheader thead'>Last Post</td><td class='minorheader thead'>Threads</td><td class='minorheader thead'>Posts</td></tr>
						";
						
						$Categories = mysql_fetch_table("SELECT * FROM `forums` WHERE `SubForum`='0' ORDER BY `OrderPos` ASC");
						
						for ($currentCategory = 0; $currentCategory < count($Categories); $currentCategory++)
						{
							if (User::HasPermission($Categories[$currentCategory], PermissionType::$ViewForum))
							{
								$PageContent .= "<tr><td colspan='5' class='majorheader'>" . $Categories[$currentCategory]["Name"] . "</td></tr><tr><td colspan='5' class='minorheader thead'>" . $Categories[$currentCategory]["Description"] . "</td></tr>";

								$CategoryForums = mysql_fetch_table("SELECT * FROM `forums` WHERE `SubForum`='{$Categories[$currentCategory]["ID"]}' ORDER BY `OrderPos` ASC");
								for ($currentCategoryForum = 0; $currentCategoryForum < count($CategoryForums); $currentCategoryForum++)
								{
									if (User::HasPermission($CategoryForums[$currentCategoryForum], PermissionType::$ViewForum))
									{
										$LastPost = GetPostByID($CategoryForums[$currentCategoryForum]["LastPostID"]);
										$PageContent .= "<tr><td class='subcontentlight'>&nbsp;</td><td class='subcontentlight'><a href='?p=forumdisplay&id=" . $CategoryForums[$currentCategoryForum]["ID"] . "'>" . $CategoryForums[$currentCategoryForum]["Name"] . "</a><br />" . $CategoryForums[$currentCategoryForum]["Description"] . "</td><td class='subcontentlight'>";

										if (User::HasPermission($CategoryForums[$currentCategoryForum], PermissionType::$ViewThreads))
										{
											if ($LastPost)
											{
												$TheThread = GetThreadByID($LastPost["ThreadID"]);
												$TheUser = GetUserByID($LastPost["PosterID"]);
												$PageContent .= "<a href='?p=viewthread&id=" . $TheThread["ID"] . "'>" . $TheThread["Name"] . "</a><br />by <a href='?p=user&id=" . $TheUser["ID"] . "'>" . $TheUser["Username"] . "</a><br /><div align='right'>" . date("F d Y, h:i A", $TheThread["LastPostTime"]) . "</div>";
											}
											else
											{
												$PageContent .= "No Posts";
											}
										}
										else
										{
											$PageContent .= "You do not have permission to view threads inside this forum.";
										}

										$PageContent .= "</td><td class='subcontentlight'>" . $CategoryForums[$currentCategoryForum]["ThreadCount"] . "</td><td class='subcontentlight'>" . $CategoryForums[$currentCategoryForum]["PostCount"] . "</td></tr>";
									}
								}
							}
						}

		$PageContent .= "
					</table>
					<br />
					<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'>
						<tr><td class='majorheader'>Forum Information</td></tr>
						<tr><td class='minorheader thead'>Online Users</td></tr>
						<tr><td class='subcontentlight'>There are currently 0 users online.</td></tr>
						<tr><td class='minorheader thead'>Statistics</td></tr>
						<tr><td class='subcontentlight'>
						Threads: 0<br />
						Posts: 0<br />
						Members: 0<br />
						Active Members: 0<br />
						Welcome to our newest member, [NAME]
						</td></tr>
					</table>
		";
	}
	break;
	
	case 'viewthread':
	{
		// Prevent any kind of injection via the id and page.
		$_GET['id'] = isset($_GET['id']) ? intval($_GET['id']) : 0;
		$_GET['page'] = isset($_GET['page']) ? intval($_GET['page']) : 1;
		
		if (User::HasPermission(GetForumByID(GetThreadByID($_GET['id'])['ForumID']), PermissionType::$ViewThreads))
		{
			// Thread view count check / increment.
			if (User::IsLoggedIn())
			{
				// Check for an existing view marker.
				$PreviousView = mysql_fetch_array(mysql_query("SELECT * FROM `views` WHERE `ThreadID`='" . $_GET['id'] . "' AND `UserID`='" . User::$Info['ID'] . "'"));
				
				// If this is the first view of the thread create one.
				if ($PreviousView == null)
				{
					mysql_query("INSERT INTO `views` (`ThreadID`, `UserID`, `ViewTime`) VALUES ('" . $_GET['id'] . "', '" . User::$Info['ID'] . "', '" . time() . "')");
					mysql_query("UPDATE `threads` SET `ViewCount`=`ViewCount`+'1' WHERE `ID`='" . $_GET['id'] . "'");
				}
				// Otherwise update last view time of this thread for this user.
				else
				{
					mysql_query("UPDATE `views` SET `ViewTime`='" . time() . "' WHERE `ThreadID`='" . $_GET['id'] . "' AND `UserID`='" . User::$Info['ID'] . "'");
				}
			}
			
			$TheThread = GetThreadByID($_GET['id']);
			$TheForum = GetForumByID($TheThread["ForumID"]);
			$PostOffset = ( ($_GET['page'] - 1) * $PostsPerPage);
			$Posts = mysql_fetch_table("SELECT * FROM `posts` WHERE `ThreadID`='" . $_GET['id'] . "' ORDER BY `ID` ASC LIMIT " . $PostOffset . "," . $PostsPerPage);
			$TotalNumPosts = mysql_num_rows(mysql_query("SELECT * FROM `posts` WHERE `ThreadID`='" . $_GET['id'] . "'"));
			$TotalNumPages = ceil($TotalNumPosts / $PostsPerPage);
			$LastShownPost = ($PostOffset + $PostsPerPage);

			$PageContent .= "<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'><tr><td class='subcontentdark'>" . ($TheThread["Locked"] ? "[Locked]" : "<a href='?p=newpost&tid=" . $_GET['id'] . "'>Reply To Thread</a>") . "</td><td align='right' class='subcontentdark'>" . generate_page_listing($PostOffset, $LastShownPost, $TotalNumPosts, $TotalNumPages, $_GET['page'], $_GET['id'], "viewthread") . "</td></table>
			
			<br />
			
			<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'><tr><td class='minorheader'><b>Thread: <a href='?p=viewthread&id=" . $TheThread["ID"] . "'>" . $TheThread["Name"] . "</a></b><br /></td></tr></table>
			<br />";

			for ($currentPost = 0; $currentPost < count($Posts); $currentPost++)
			{
				$ParsedPostContent = $Posts[$currentPost]["PostContent"];
				$ParsedPostContent = str_replace("\n", "<br />", $ParsedPostContent); // Process intended linebreaks.
				$Poster = GetUserByID($Posts[$currentPost]["PosterID"]);
				$PageContent .= "
						<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'>
							<tr><td colspan='2' class='minorheader thead'><div style='float: left;'>" . date("l F d Y, h:i A", $Posts[$currentPost]["PostTime"]) . "</div><div style='float: right;'>#" . ($PostOffset + $currentPost + 1) . "</div></td></tr>
							<tr><td class='subcontentlight'><div style='width: 150px; float: left;'><a href='?p=user&id=" . $Poster["ID"] . "'>" . $Poster["Username"] . "</a><br />" . UserGroupNameFromID($Poster["UserGroup"]) . "<br /><br />Posts: " . $Poster["PostCount"] . "<br />Join Date: " . date("F d, Y", $Poster["JoinDate"]) . "</div><div style='float: left;'>" . $ParsedPostContent . "</div></td></tr>
							<tr><td colspan='2' class='subcontentdark'><div align='right'>additional actions</div></td></tr>
						</table>
				";
				if ($currentPost + 1 < count($Posts))
				{
					$PageContent .= "<br />";
				}
			}

			addBreadcrumb("<a href='?p=forumdisplay&id=" . $TheForum["ID"] . "'>" . $TheForum["Name"] . "</a>");
			addBreadcrumb("<a href='?p=viewthread&id=" . $TheThread["ID"] . "'>" . $TheThread["Name"] . "</a>");
		}
		else
		{
			$PageContent .= "<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'><tr><td class='subcontentlight'>";
			$PageContent .= "<center>You do not have permission to view this thread's content.</center>";
			$PageContent .= "</td></tr></table>";
		}
	}
	break;
	
	case 'forumdisplay':
	{
		// Prevent any kind of injection via the id and page.
		$_GET['id'] = isset($_GET['id']) ? intval($_GET['id']) : 0;
		$_GET['page'] = isset($_GET['page']) ? intval($_GET['page']) : 1;
		
		$TheForum = GetForumByID($_GET['id']);
		if (User::HasPermission($TheForum, PermissionType::$ViewForum))
		{
			$ThreadOffset = ( ($_GET['page'] - 1) * $ThreadsPerPage);
			$Threads = mysql_fetch_table("SELECT * FROM `threads` WHERE `ForumID`='" . $_GET['id'] . "' ORDER BY `LastPostTime` DESC LIMIT " . $ThreadOffset . "," . $ThreadsPerPage);
			$TotalNumThreads = mysql_num_rows(mysql_query("SELECT * FROM `threads` WHERE `ForumID`='" . $_GET['id'] . "'"));
			$TotalNumPages = ceil($TotalNumThreads / $ThreadsPerPage);
			$LastShownThread = ($ThreadOffset + $ThreadsPerPage);

			$PageContent .= "
			<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'>
			<tr><td class='majorheader'><b>Forum: <a href='?p=forumdisplay&id=" . $TheForum["ID"] . "'>" . $TheForum["Name"] . "</a></b></td></tr>
			<tr><td class='minorheader thead'>" . $TheForum["Description"] . "</td></tr>
			</table>
			<br />
			<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'><tr><td class='subcontentdark'><a href='?p=newpost&fid=" . $_GET['id'] . "'>Post New Thread</a></td><td align='right' class='subcontentdark'>" . generate_page_listing($ThreadOffset, $LastShownThread, $TotalNumThreads, $TotalNumPages, $_GET['page'], $_GET['id'], "forumdisplay") . "</td></table>
			<br />

						<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'>
							<tr><td class='minorheader thead'>&nbsp;</td><td class='minorheader thead'>Title / Thread Starter</td><td class='minorheader thead'>Replies / Views</td><td class='minorheader thead'>Last Post By</td></tr>";

			if (User::HasPermission($TheForum, PermissionType::$ViewThreads))
			{
				if ($TotalNumThreads > 0)
				{
					for ($currentThread = 0; $currentThread < count($Threads); $currentThread++)
					{
						// NOTE: Maybe some checks for stuff being the same could be an optimization here.
						$FirstPost = GetPostByID($Threads[$currentThread]["FirstPostID"]);
						$FirstPoster = GetUserByID($FirstPost["PosterID"]);
						$LastPost = GetPostByID($Threads[$currentThread]["LastPostID"]);
						$LastPoster = GetUserByID($LastPost["PosterID"]);

						$PageContent .= "<tr><td class='subcontentlight'>&nbsp;</td><td class='subcontentlight'>" . ($Threads[$currentThread]["Locked"] ? "[Locked] " : "") . "<a href='?p=viewthread&id=" . $Threads[$currentThread]["ID"] . "'>" . $Threads[$currentThread]["Name"] . "</a><br />Started by <a href='?p=user&id=" . $FirstPoster["ID"] . "'>" . $FirstPoster["Username"] . "</a>, " . date("F d Y, h:i A", $FirstPost["PostTime"]) . "</td><td class='subcontentlight'>Replies: " . ($Threads[$currentThread]["PostCount"] - 1) . "<br />Views: " . $Threads[$currentThread]["ViewCount"] . "</td><td class='subcontentlight'><a href='?p=user&id=" . $LastPoster["ID"] . "'>" . $LastPoster["Username"] . "</a><br />" . date("F d Y, h:i A", $LastPost["PostTime"]) . "</td></tr>";
					}
				}
				else
				{
					$PageContent .= "<tr><td colspan='4' class='subcontentlight'><i><center>There are no threads in this forum yet.</center></i></td></tr>";
				}
			}
			else
			{
				$PageContent .= "<tr><td colspan='4' class='subcontentlight'><i><center><i>You do not have permission to view threads inside this forum.</i></center></i></td></tr>";
			}

			$PageContent .= "
						</table>

			";

			addBreadcrumb("<a href='?p=forumdisplay&id=" . $TheForum["ID"] . "'>" . $TheForum["Name"] . "</a>");
		}
		else
		{
			$PageContent .= "<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'><tr><td class='subcontentlight'><center>You do not have permission to view this forum's content.</center></td></tr></table>";
			addBreadcrumb("Error");
		}
	}
	break;
	
	case 'user':
	{
		// Prevent any kind of injection via the id.
		$_GET['id'] = intval($_GET['id']);
		
		$TheUser = GetUserByID($_GET['id']);
		
		$PageContent .= "<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'><tr><td class='subcontentlight'>";

		$PageContent .= "Username: " . $TheUser["Username"] . "<br />";
		$PageContent .= "User group: " . $TheUser["UserGroup"] . "<br />";
		$PageContent .= "Posts: " . $TheUser["PostCount"] . "<br />";
		$PageContent .= "Last Login: " . $TheUser["LastLogin"] . "<br />";
		$PageContent .= "Last Action: " . $TheUser["LastAction"] . "<br />";
		
		$PageContent .= "</td></tr></table>";
		
		addBreadcrumb("<a href='?p=user&id=" . $TheUser["ID"] . "'>" . $TheUser["Username"] . "'s Profile</a>");
	}
	break;
	
	case 'login':
	{
		if (isset($_POST['Login']))
		{
			$User = mysql_real_escape_string($_POST['Username']);
			$Pass = mysql_real_escape_string($_POST['Password']);
			
			$RedirectPage = "?p=home";
			
			switch (User::Login($User, $Pass))
			{
				case 1		: $LoginMessage = "You have logged in successfully."; break;
				case -1		: $LoginMessage = "You are already logged in."; break;
				case 0		: $LoginMessage = "User doesn't exist or incorrect password."; $RedirectPage = "?p=login"; break;
				default		: $LoginMessage = "An unidentified error occurred. Please try again."; $RedirectPage = "?p=login"; break;
			}
			
			$PageContent .= "<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'><tr><td class='subcontentlight'><center>" . $LoginMessage . "<br />You will be redirected in 3 seconds.</center></td></tr></table>";
			redirect($RedirectPage, 3);
		}
		else
		{
			$PageContent .= "
			<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'>
			<tr><td colspan='2' class='majorheader'><center>Login</center></td></tr>
			<tr><td class='subcontentlight'>
				<table border='0' align='center'>
				<form method='post' action='?p=login'>
					<tr><td align='right'>Username: </td><td><input type='text' name='Username'></input></td></tr>
					<tr><td align='right'>Password: </td><td><input type='password' name='Password'></input></td></tr>
					<tr><td>&nbsp;</td><td><input name='Login' type='submit' value='Login'></input></td></tr>
				</form>
				</table>
			</td></tr>
			</table>
			";
		}

		addBreadcrumb("<a href='?p=login'>Login</a>");
	}
	break;
	
	case 'logout':
	{
		$PageContent .= "<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'><tr><td class='subcontentlight'>";
		$PageContent .= (User::Logout() ? "You have logged out successfully." : "You are not logged in.");
		$PageContent .= "</td></tr></table>";
		
		addBreadcrumb("<a href='?p=logout'>Logout</a>");
	}
	break;
	
	case 'newpost':
	{
		// Determine if this is an attempt to post to an existing thread or make a new one.
		$GivenFID = (isset($_GET['fid']) ? intval($_GET['fid']) : "NA");
		$GivenTID = (isset($_GET['tid']) ? intval($_GET['tid']) : "NA");
		$PostingError = "";
		$PostingType = -1;
		
		if ($GivenFID != "NA")
		{
			$ForumID = $GivenFID;
			$TheForum = GetForumByID($ForumID);
			
			if ($TheForum == null)
			{
				$PostingError = "The specified forum with ID " . $ForumID . " does not exist.";
			}
			else if (!User::HasPermission($TheForum, PermissionType::$CreateThreads))
			{
				$PostingError = "You do not have permission to create threads in this forum.";
			}
			
			$PostingType = PostType::$NewThread;
		}
		else if ($GivenTID != "NA" && $GivenFID == "NA")
		{
			$ThreadID = $GivenTID;
			$TheThread = GetThreadByID($ThreadID);
			
			if ($TheThread == null)
			{
				$PostingError = "The specified thread with ID " . $ThreadID . " does not exist.";
			}
			else
			{
				$TheForum = GetForumByID($TheThread["ForumID"]);
				if ($TheForum == null)
				{
					$PostingError = "A critical error occurred, a thread exists in a forum that doesn't. Please contact the administrator mentioning this error and the following information:<br />Thread ID: " . $ThreadID . "<br />Forum ID: " . $TheThread["ForumID"];
				}
				else if (!User::HasPermission($TheForum, PermissionType::$Post))
				{
					$PostingError = "You do not have permission to make new posts in this thread.";
				}
			}
			
			if ($TheThread["Locked"])
			{
				$PostingError = "This thread is locked, you cannot post in it!";
			}
			
			$PostingType = PostType::$NewPost;
		}
		else
		{
			$PostingError = "Please do not attempt to modify a posting query. The requested operation was invalid.";
		}
		
		$PageContent .= "
					<table class='subcontentcontainer' cellpadding='6' cellspacing='1' width='100%'>
					<tr><td colspan='2' class='majorheader'><center>" . ($PostingType == PostType::$NewThread ? "New Thread" : "New Post") . "</center></td></tr>
					<tr><td class='subcontentlight'><center>";
		
		// Act according to the operation we're trying to do.
		if (!$PostingError)
		{
			if (User::IsLoggedIn())
			{
				if (isset($_POST['Post']))
				{
					$TimeUntilPostingEnabled = User::$Info["LastPostTime"] - (time() - $PostFloodTimeout);

					if ($TimeUntilPostingEnabled < 0)
					{
						if ($PostingType == PostType::$NewThread)
						{
							$PostTitle = mysql_real_escape_string($_POST['PostTitle']);
						}
						$PostContent = mysql_real_escape_string($_POST['PostContent']);
						
						$TitleCheck = ( ($PostingType == PostType::$NewThread) ? ((strlen($PostTitle) > 0) ? ((strlen($PostTitle) <= $MaxThreadTitleLength) ? 0 : 2) : 1) : 0 );
						$ContentCheck = (strlen($PostContent) <= $MaxPostLength ? 0 : 1);

						if ($TitleCheck == 0 && $ContentCheck == 0)
						{
							if ($PostingType == PostType::$NewThread)
							{
								$ThreadID = MakeNewThread($ForumID, $PostTitle, $PostContent);
								$PageContent .= "New thread posted successfully... Redirecting you to the thread.";
								redirect("?p=viewthread&id=" . $ThreadID, 3);
							}
							else if ($PostingType == PostType::$NewPost)
							{
								$PostID = MakeNewPost($ThreadID, $PostContent);
								$PageContent .= "New post made successfully... Redirecting you to the post.";
								redirect("?p=viewthread&id=" . $ThreadID, 3);
							}
						}
						else
						{
							$PageContent .= "The following error(s) occurred:<br /><ul>";
							if ($ContentCheck == 1)
							{
								$PageContent .= "<li>The post length exceeded " . $MaxPostLength . " characters.</li>";
							}
							if ($TitleCheck == 1)
							{
								$PageContent .= "<li>The post title was left blank.</li>";
							}
							else if ($TitleCheck == 2)
							{
								$PageContent .= "<li>The post title exceeded " . $MaxThreadTitleLength . " characters.</li>";
							}
							$PageContent .= "</ul>";
						}
					}
					else
					{
						$PageContent .= "You are attempting to post too quickly! You cannot post until another " . $TimeUntilPostingEnabled . " second(s) have passed.";
					}
				}
				else
				{
					$PageContent .= "
						<table border='0'>
						<form method='post' action='" . ($PostingType == PostType::$NewThread ? "?p=newpost&fid=" . $ForumID : "?p=newpost&tid=" . $ThreadID) . "'>
							<tr><td align='right'>Title: </td><td><input type='text' name='PostTitle' size=110 " . ($PostingType == PostType::$NewPost ? "readonly value='" . $TheThread['Name'] . "'" : "") . "></input></td></tr>
							<tr><td align='right'>Post Content: </td><td><textarea name='PostContent' rows=15 cols=75></textarea></td></tr>
							<tr><td>&nbsp;</td><td><input name='Post' type='submit' value='Post'></input></td></tr>
						</form>
						</table>";
				}
			}
			else
			{
				$PageContent .= "You are not logged in. You need to be logged in to perform this action.<br />You will now be redirected.";
				redirect("?p=login", 3);
			}

			if ($PostingType == PostType::$NewThread)
			{
				addBreadcrumb("<a href='?p=forumdisplay&id=" . $TheForum["ID"] . "'>" . $TheForum["Name"] . "</a>");
				addBreadcrumb("<a href='?p=newpost&fid=" . $ForumID . "'>New Thread</a>");
			}
			else if ($PostingType == PostType::$NewPost)
			{
				addBreadcrumb("<a href='?p=forumdisplay&id=" . $TheForum["ID"] . "'>" . $TheForum["Name"] . "</a>");
				addBreadcrumb("<a href='?p=viewthread&id=" . $TheThread["ID"] . "'>" . $TheThread["Name"] . "</a>");
				addBreadcrumb("<a href='?p=newpost&tid=" . $ThreadID . "'>New Post</a>");
			}
		}
		else
		{
			$PageContent .= $PostingError;
			
			addBreadcrumb("Error");
		}
		
		$PageContent .= "</center>
					</td></tr>
					</table>";
	}
	break;
}

// Update the LastAction in the user account info.
if (User::IsLoggedIn())
{
	mysql_query("UPDATE `users` SET `LastAction`='" . time() . "' WHERE `ID`='" . User::$Info['ID'] . "'");
}

mysql_close($MySQL['Connection']);

PrintHeader();
echo $PageContent;
PrintFooter();

?>