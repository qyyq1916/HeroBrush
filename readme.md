### 创建基类的character
HeroCharacter为基类，具有基本的bpthirdperson的所有的特性。herobrush继承hero。
在beginplay中在基类中实现了绑定mapping，如果在继承的新类中需要添加新的内容直接在setupcomp中绑定即可。

hero的基类中实现了一个方法，设置发射的location和发射目标的location，通过GetAimRotation，计算得到两者之间的rotation。在后续实现远程射击中有帮助。

### 有关同步资产，对于包内部的有做修改的内容，我们把所有的修改的文件传到共享盘的update_to_latest文件夹下！！

### 子弹系统
子弹系统由基类BaseWeaponBullets生成，默认的碰撞效果是Projectile,如果在preset的collisioni中没有设置，可以重新设置这个Projectile。

默认的子弹是使用/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Primary/FX/P_Gideon_Primary_Projectile.P_Gideon_Primary_Projectile

实现消失与爆炸效果，需要在Bp_BaseWeaponBullets中复制所有的逻辑到你的蓝图类中。

子弹的瞄准：
    在character类中提供了GetAimRotation()的方法会自动修改属性AimRotation。使用前需要设置好FromLocation和AimLocation。
eg:
```cpp
    const FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FromLocation = HandLocation; // 设置开始的rotation
    AimLocation = GetActorLocation(); // 设置打击目标的location
    GetAimRotation(); // 得到rotation
```
在设置好后，GetWorld()->SpawnActor< xxx >(FromLocation, AimRotation); 或 GetWorld()->SpawnActor<xxx>(FromLocation, GetAimRotation());
HeroBrush中的AimLocation,在蓝图中通过射线检测获得屏幕中心的射线打到的第一个物品的位置作为AimLocation.在components文件架下有对应的蓝图，如果在人物蓝图的函数中没有laser，可以复制过去后，连接那个set。



#### 流程：
发射物品：首先根据BaseWeaponBullets生成定制化的蓝图类，并且设置自己想要的射击物品。复制Bp_BaseWeaponBullets的蓝图逻辑实现触碰爆炸（simulate physics && ProjectileGravityScale = 0 默认如此) ，可以设置爆炸的效果


发射：修改你的发射的fromlocation 和 aimlocation,调节间隔时间。样例如下：
```cpp
.h
UPROPERTY(VisibleAnywhere)
		FTimerHandle TimerHandle_PrimaryAttack;


.cpp
//Basic Attack
void AHeroBrushCharacter::Primary_Attack() {

	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AHeroBrushCharacter::PrimaryAttack_TimeElapsed, 0.1f);
}

// you can set shoot location and shoot speed
void AHeroBrushCharacter::PrimaryAttack_TimeElapsed() {
	// 找到一个发射位置的骨骼体
	const FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FromLocation = HandLocation; // 设置开始的rotation
	GetAimRotation(); // 得到rotation

	const FTransform SpawnTM = FTransform(AimRotation, FromLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams); // 使用已有的蓝图加载，在蓝图中设置
}
```

绑定按键：PlayerInputComponent->BindAction("Primary_Attack", IE_Pressed, this, &AHeroBrushCharacter::Primary_Attack);
在蓝图类中设置好绑定的发射物品



### 伤害系统
#### BaseWeaponBullets
由默认的第一人称视角修改得到。设置DamageFrom来反应子弹来源，BulletDamage表示该子弹的伤害，新建一个伤害的类型，定义一种新的状态（使用int类型）

这里新建了一个AEnemyBulletsBase的类，这个是敌人的默认投掷物，只改变了bool值和伤害。


#### character的伤害
修改了伤害的作用，全部将伤害做到子弹中，在蓝图中的修改了伤害的判定来源。也就是远程的伤害是子弹获取到的。而aoe也来进行判定。而连续的判定依旧在changehealth中。
但是在蓝图中进行调用

#### 血量显示
角色的widget，在level中显示，这个Status中获取到角色的生命和能量信息，实时更新。如果有血量变化，在角色中直接改变原本的值即可。后续可以添加角色具体的血量值。

#### AOE技能设计
粒子效果的加入，在动作的蒙太奇中加入，同时可以加入设置notify事件，然后在动画蓝图中调用这个事件发生的时候，进行函数的调用。使其生成伤害判定范围。

效果是动作自带的，而伤害判定范围是生成的。

我们定义AOEitem来创建一个领域，默认为400radius的几乎完全overlap的collision，comp。

定义伤害事件，首先我们还是一样，使用一个bool来判断伤害来源，使用int类型来判断AOE的伤害是什么，伤害多少。

当使用这个AOE伤害的时候，可以仿写下面的代码：
```cpp
void AHeroBrushCharacter::AOE_Attack()
{
	PlayAnimMontage(AOEAnim);	
}
void AHeroBrushCharacter::AOE_Attack_TimeElapsed()
{
	AAOEItem* tempAoe = GetWorld()->SpawnActor<AAOEItem>(GetActorLocation(), GetActorRotation());
}
```
直接在这个角色的位置生成这个aoe的范围。当然后面可以做成一个继承后的蓝图类，在代码中使用subclass，选中生成。

伤害判定：
```cpp
// aoe的来源
	auto actor1 = Cast<AAOEItem>(OtherActor);
	if (actor1 != nullptr && actor1->DamageFrom) {
		if (actor1->AOEInfo == 0) {
			ChangeHealth(false, -1, actor1->AOEDamage);
		}
	}
```
aoe的特效一律加载到montage中。

##### 连续的伤害
连续的伤害aoe类型，写在BIG_AOE_Player1的蓝图中。