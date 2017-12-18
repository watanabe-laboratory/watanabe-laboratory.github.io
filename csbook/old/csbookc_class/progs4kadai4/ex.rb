# 森林火災の実験
# ex.rb

# 実験の設定：固定パラメータ
n = 50;  nt = (2*n+1)**2;  limit = nt * 0.001 # 全焼の基準 (0.1%)

# 実験用パラメータ
ex = 30;  b = 5;  p = 10;  time = 100;  seed = 1

# 実験
   bcount = 0
   for i in (1..ex)
      result = `./fire.exe #{n} #{p} #{b} #{time} #{seed}`
      r1, r2, r3 = result.chomp.split(/\s*,\s*/)
      t  = r1.to_i
      nb = r2.to_i
      nu = r3.to_i
      if nu <= limit
         bcount = bcount + 1
      end
      print(t, ", ", nu, ": ", bcount, "\n")
      seed = seed + 1;  i = i + 1
   end
   R = bcount.to_f / ex.to_f
   print(p, ", ", R, "\n")
