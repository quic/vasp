# Implement your custom attack

> This tutorial assumes the root folder to be `<path/to/veins>/src/vasp`.

1. Create your attack class files in the `attack` folder, choose appropriate subfolder structure.
2. Use the `vasp::attack` namespace. Your namespace should match the folder structure.
    > [`attack/acceleration/Constant.h`](../attack/acceleration/Constant.h) attack is under the `vasp::attack::acceleration` namespace.
3. All attacks inherit from the [attack interface](../attack/Interface.h) ([`attack/Interface.h`](../attack/Interface.h))
    > Hence all attacks need to implement the `attack()` method
4. Implement your attack functionality in the `attack()` method. Any variables that need to be updated before
    running the attack should be either updated via a dedicated constructor of the attack or an `update()` method.
    > See [`attack/acceleration/ConstantOffset.h`](../attack/acceleration/ConstantOffset.h) and [`attack/acceleration/ConstantOffset.cc`](../attack/acceleration/ConstantOffset.cc) for example.
5. Make sure to set the attack type appropriately using the `bsm->setAttackType(<attack_type>)` method inside your `attack()` method.
6. Add an `enum` for your attack in [`attack/Type.h`](../attack/Type.h) file. Note down the corressponding integer value of your attack's enum.
7. Call your attack in the [`driver/CarApp.cc`](../driver/CarApp.cc) class.
    * Call your attack in the `injectAttack()` method if your attack modifies the attacker's own kinematic information.
        * Use proper switch-case based on your attack's `enum`.
        * Assign your attack's constructor to the `attack_` variable.
    * Call your attack in the `injectGhostAttack()` method if your attack creates a ghost vehicle to perform the attack.
        * Use proper switch-case based on your attack's `enum`.
        * Assign your attack's constructor to the `ghostAttack_` variable.
8. Assign your attack's `enum`'s integer equivalent value to the `attackType` variable in the [`scenario/omnetpp.ini`](../scenario/omnetpp.ini) file.
9. Run simulation by following the the "Running simulations" instructions in the [README](../README.md)
10. Once the simulation has ended, open the latest `rxtrace-*.csv` file in `scenario/results` folder and observe the data to check for correctness.
    > We recommend Microsoft Excel (or equivalent) program to observe the file and perform rudimentary analyses.
