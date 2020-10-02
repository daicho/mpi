foreach i (`seq 1 30`)
    foreach j (`seq 1 10`)
        mpirun -np ${i} kadai01 4000000000 > ./result/${i}-${j}.txt
    end
end
