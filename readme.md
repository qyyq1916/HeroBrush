#### 创建基类的character
HeroCharacter为基类，具有基本的bpthirdperson的所有的特性。herobrush继承hero。
在beginplay中在基类中实现了绑定mapping，如果在继承的新类中需要添加新的内容直接在setupcomp中绑定即可。

#### 子弹系统
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
在设置好后，GetWorld()->SpawnActor<xxx>(FromLocation, AimRotation); 或 GetWorld()->SpawnActor<xxx>(FromLocation, GetAimRotation());
HeroBrush中的AimLocation,在蓝图中通过射线检测获得屏幕中心的射线打到的第一个物品的位置作为AimLocation.在components文件架下有对应的蓝图，如果在人物蓝图的函数中没有laser，可以复制过去后，连接那个set。

##### 流程：
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