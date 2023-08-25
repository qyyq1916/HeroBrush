### 创建基类的character
HeroCharacter为基类，具有基本的bpthirdperson的所有的特性。herobrush继承hero。
在beginplay中在基类中实现了绑定mapping，如果在继承的新类中需要添加新的内容直接在setupcomp中绑定即可。

hero的基类中实现了一个方法，设置发射的location和发射目标的location，通过GetAimRotation，计算得到两者之间的rotation。在后续实现远程射击中有帮助。



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
我们将原本character中的health系统放到基类中，敌人和自身都需要health系统，在继承之后重新设置。

当接触到bullets之后NotifyActorBeginOverlap，调用CheckTouchActor，检测接触到的是来自于谁的什么类型的子弹，使用接口changeHealth,控制变化血量的长短时间和血量（目前没有加入长时间的具体代码）。后续可以添加别的效果。

销毁由子弹自动销毁不需要角色控制
#### 血量显示
角色的widget，在level中显示，这个Status中获取到角色的生命和能量信息，实时更新。如果有血量变化，在角色中直接改变原本的值即可。后续可以添加角色具体的血量值。


