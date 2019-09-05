eval "$(
declare | LC_ALL=C grep 'test.*=' | while read opt; do
	echo "echo Processing $opt"
	optname=${opt%%=*}
	echo "echo Optname is $optname"
	optvalue=${opt#*=}
	echo "echo Optvalue is: $optvalue"
	echo "echo $optname=$optvalue"
done
)"
