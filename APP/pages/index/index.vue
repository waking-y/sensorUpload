<template>
	<view class="wrap">
		<view class="dev-area">
			<view class="dev-cart">
				<view class="dev-name">
					<view class="">温度</view>
					<image class="dev-logo" src='/static/temp(1).png' mode=""></image>
				</view>
				<view class="dev-data">{{temp}}℃</view>
			</view>
			<view class="dev-cart">
				<view class="dev-name">
					<view class="">湿度</view>
					<image class="dev-logo" src='/static/humi.jpg' mode=""></image>
				</view>
				<view class="dev-data">{{humi}}%</view>
			</view>
			<view class="dev-cart">
				<view class="dev-name">
					<view class="">led</view>
					<image class="dev-logo" src='/static/led.jpg' mode=""></image>
				</view>
				<switch :checked="led" @change="onLedSwitch" color="#007aff"/>
			</view>
		</view>
	</view>

</template>

<script>
	const {
		createCommonToken
	} = require('@/key.js')
	export default {
		data() {
			return {
				temp: '11',
				humi: '22',
				led: false,
				token: '',
			}
		},
		onLoad() {
			const params = {
				author_key: '360845d4b1e94a749bb0ce8a251864a7',
				version: '2022-05-01',
				user_id: '502508',

			}
			this.token = createCommonToken(params);
			console.log(this.token)
		},
		onShow() {
			this.fetchDevData();
			setInterval(() => {
				this.fetchDevData();
			}, 3000)
		},
		methods: {
			fetchDevData() {
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/query-device-property',
					method: 'GET',
					data: {
						text: 'uni.request',
						product_id: 'ynlW3p1H1D',
						device_name: 'd1'
					},
					header: {
						'authorization': this.token //自定义请求头信息
					},
					success: (res) => {
						console.log(res.data);
						console.log(res.data.data[0].value);
						this.temp = res.data.data[2].value;
						this.humi = res.data.data[0].value;
						this.led = res.data.data[1].value === "true";
					}
				})
			},
			onLedSwitch(event) {
				let value = event.detail.value;
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property',
					method: 'POST',
					data: {
						product_id: 'ynlW3p1H1D',
						device_name: 'd1',
						params: {
							"led": value
						}
					},
					header: {
						'authorization': this.token //自定义请求头信息
					},
					success: (res) => {
						console.log('LED' + (value ? ON : OFF) + '!');

					}
				})
			}
		}
	}
</script>

<style>
	.wrap {
		padding: 30rpx;
	}

	.dev-area {
		display: flex;
		justify-content: space-between;
		flex-wrap: wrap;
	}

	.dev-cart {
		height: 150rpx;
		width: 320rpx;
		border-radius: 30rpx;
		margin-top: 30rpx;
		display: flex;
		justify-content: space-around;
		align-items: center;
		box-shadow: 0 0 15rpx #ccc;
	}

	.dev-name {
		font-size: 20rpx;
		text-align: center;
		color: #6d6d6d;
	}

	.dev-logo {
		width: 70rpx;
		height: 70rpx;
		margin-top: 10rpx;
	}

	.dev-data {
		font-size: 50rpx;
		color: #6d6d6d;
	}

</style>