all:
	chmod +x make.sh
	./make.sh

iso:
	chmod +x makeiso.sh
	./makeiso.sh

clean:
	rm -f JOS.iso
	rm -f JOS.img