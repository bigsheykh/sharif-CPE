output:
	@mkdir -p out
	@cp parameters.conf out
	@cd out; \
	g++ ../fixed.cpp -o fixed; \
	g++ ../exp.cpp -o exp; \
	cd ..
run:
	@cd out; \
	./fixed; \
	./exp; \
	cd ..
clear:
	@rm -rf out
