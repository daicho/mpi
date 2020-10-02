foreach i (`seq 1 30`)
    foreach j (`seq 1 10`)
        mpirun -np ${i} kadai03 200000000 > ./kadai03-result/${i}-${j}.txt
    end
end
