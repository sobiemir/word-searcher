if g++ "./main.cpp" "./src/textbox.cpp" "./src/searcher.cpp" "./src/panel.cpp" "./src/interface.cpp" \
	-lncurses -o "wordsearcher"; then
	echo "WordSearcher compiled successfully"
else
	echo "Compile failed for WordSearcher"
fi
