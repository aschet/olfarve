mod olfarve;

use crate::olfarve::olfarve::srm_to_srgb;
use crate::olfarve::olfarve::rgb_to_hex;
use crate::olfarve::olfarve::DEFAULT_PATH;

fn main() {
    println!("SRM,sRGB");
    for i in 1..=50 {
        let srgb = srm_to_srgb(i as f32, DEFAULT_PATH);
        println!("{},{:?}", i, rgb_to_hex(srgb));
    }
}