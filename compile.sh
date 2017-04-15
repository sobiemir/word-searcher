MAIN="main"

if g++ "./src/$MAIN.cpp" -o "wordsearcher"; then
	echo "$MAIN compiled successfully"
else
	echo "Compile failed for $MAIN"
fi
