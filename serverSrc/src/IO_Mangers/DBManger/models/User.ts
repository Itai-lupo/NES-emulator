import  mongoose from 'mongoose';
import bcrypt from 'bcryptjs'; 
import jwt from 'jsonwebtoken';

const userSchema = new mongoose.Schema({
    name: {
        type: String,
        required: [true, 'Please add name']
    },
    email: {
        type: String,
        required: [true, 'Please add email'],
        unique : true,
        dropDups: true,
        match: [
            /^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/,
            'Please add a valid email'
        ]
    },
    password: { 
        type: String,
        required: [true, 'Please add password'],
        minlength: 6,
        select: false
    },
    restPasswordToken: String,
    restPasswordExpire: Date,
    createdAt: {
        type: Date,
        default: Date.now()
    }
});


userSchema.pre('save', async function(next)
{
    const salt = await bcrypt.genSalt(10);
    (this as any).password = await bcrypt.hash((this as any).password, salt);
});

userSchema.methods.getSignedJwtToken = function()
{
    return jwt.sign({ id: this.id }, process.env.JWT_SECRET, {
       expiresIn: process.env.JWT_EXPIRE
   });
}


userSchema.methods.matchPassword = async function(enteredPassword)
{
   return await bcrypt.compare(enteredPassword, (this as any).password);
}


userSchema.set('toObject', { virtuals: true })
userSchema.set('toJSON', { virtuals: true })

export default mongoose.model("User", userSchema);