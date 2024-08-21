工具自动替换
Linux下替换
 ./change_puk --teebin tee-pager.bin
该命令将自动生成一个2048长度的RSA密钥oem_privkey.pem并保存在当前目录下，并自动使用该密钥中的公钥替换tee-pager.bin中的原始公钥
./change_puk --teebin tee-pager.bin --key oem_privkey.pem
使用客户指定的密钥中的公钥来替换tee-pager.bin中的原始公钥，密钥长度须2048长度



windows下替换
打开change_public_key.exe点击"生成oem_privkey.pem"按钮，生成并保存密钥到当前目录
选择刚刚生成的密钥和需要修改的tee-pager.bin镜像，点击"修改公钥"将自动使用密钥中的公钥替换tee-pager.bin中的原始公钥


注意：
1.由于change_public_key.exe会调用BouncyCastle.Crypto.dll第三方库，请确保BouncyCastle.Crypto.dll与change_public_key.exe在同一目录下。
2.以上替换的公钥是用于OPTEE验证TA应用合法性的，用户在生成新密钥后也需要替换optee_test/export-user_ta/keys/oem_privkey.pem并重新编译TA
	该密钥在编译TA时会对TA重新进行签名。