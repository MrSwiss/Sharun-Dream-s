static const char *mime_type[][2] = {
	{".bin", "application/octet-stream"}, // must stay frist
	{".exe", "application/octet-stream"},
	{".dll", "application/octet-stream"},

	{".xhtml", "application/xhtml+xml"},
	{".html",  "text/html"},
	{".js",    "application/javascript"},
	{".css",   "text/css"},
	{".csv",   "text/csv"},
	{".txt",   "text/plain"},
	{".json",  "application/json"},
	{".xml",   "application/xml"},
	{".swf",   "application/x-shockwave-flash"},

	{".ico",   "image/x-icon"},
	{".gif",   "image/gif"},
	{".jpg",   "image/jpeg"},
	{".jpeg",  "image/jpeg"},
	{".png",   "image/png"},
	{".svg",   "image/svg+xml"},

	{".x-flv",   "video/x-flv"},
	{".mp4",   "video/mp4"},
	{".webm",  "video/webm"},

	{".ogg",   "application/ogg"},
	{".mp3",   "audio/mp3"},

	{".zip",   "application/zip"},
	{".rar",   "application/rar"},
	{".7z",    "application/7z"},

	{".pdf",   "application/pdf"},

	{NULL, NULL} // must stay at end
};

