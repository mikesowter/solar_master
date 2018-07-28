void setupServer() {
  server.on ( "/", handleMetrics );
  server.on ( "/dir", handleDir );
  server.on ( "/metrics", handleMetrics );
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println( "server started" );
  if ( MDNS.begin ( "solar" ) ) Serial.println ( "MDNS solar" );
}
