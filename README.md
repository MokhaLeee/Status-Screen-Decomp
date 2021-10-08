# Mokha's FE8U Status Screen Decomp
A program interpretation of the vanilla FE8U Status Screen. Also compilable by EA.


## how to take it in rom

Read [FEU Buildfile Tutorial](https://tutorial.feuniverse.us/intro).

- Place FE8U clean rom named as "FE8_clean.gba" in your path.
- Place [Event Assembler](https://feuniverse.us/t/event-assembler/1749) in your path.
- Click `MAKE HACK.cmd`.

## how to compile C
Read [C-Tutorial by StanH](https://feuniverse.us/t/guide-doc-asm-hacking-in-c-with-ea/3351).

- Proper install DevkitPro in `c:/`.
- Put [lyn asset](https://feuniverse.us/t/ea-asm-tool-lyn-elf2ea-if-you-will/2986) in `src/Tools/`
- Open Msys2 bash and cd folder `src`.
- `sh build.sh`!

首先需要将lyn组件放进/src/Tools文件夹里。剩下的步骤和我之前发在B站的[编译视频（P2）](https://www.bilibili.com/video/BV1hq4y1P7am) 是一致的。

## 一些碎碎念
- 整个界面展示都是采用tile来做的，其中在绘制字串和Icon的过程，控制的x与y都对应的是8*8的格子。
- gpStatScreenPageBg0Map与gpStatScreenPageBg2Map都是[][0x20]的数组，20H->32D，相当于一行对应0x20*0x8=0x100=256个点。

 