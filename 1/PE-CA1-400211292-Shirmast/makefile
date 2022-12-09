output: clear
	@mkdir -p out
	@cp parameters.conf out
	@cd out; \
	g++ ../fixed.cpp -o fixed -std=c++11 -fopenmp -lpthread; \
	g++ ../exp.cpp -o exp -std=c++11 -fopenmp -lpthread; \
	cd ..

run:
	@cd out; \
	./fixed; \
	cd ..

run_all: output
	@cd out; \
	./fixed run_all; \
	./exp; \
	cd ..

clear:
	@rm -rf out
